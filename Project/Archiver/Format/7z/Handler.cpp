// 7z/Handler.cpp

#include "StdAfx.h"

#include "Handler.h"

#include "Windows/Time.h"
#include "Windows/COMTry.h"

#include "Common/Defs.h"
#include "Common/CRC.h"
#include "Common/StringConvert.h"

#include "../../../Compress/Interface/CompressInterface.h"

#include "Archive/Common/ItemNameUtils.h"

using namespace std;

using namespace NArchive;
using namespace N7z;

using namespace NWindows;
using namespace NTime;

CHandler::CHandler()
{
  Init();
}

STDMETHODIMP CHandler::EnumProperties(IEnumSTATPROPSTG **anEnumProperty)
{
  #ifndef _SFX
  COM_TRY_BEGIN
  CComObjectNoLock<CEnumArchiveItemProperty> *anEnumObject = 
      new CComObjectNoLock<CEnumArchiveItemProperty>;
  if (anEnumObject == NULL)
    return E_OUTOFMEMORY;
  CComPtr<IEnumSTATPROPSTG> anEnum(anEnumObject);
  anEnumObject->Init(m_Database.m_ArchiveInfo.FileInfoPopIDs);
  return anEnum->QueryInterface(IID_IEnumSTATPROPSTG, (LPVOID*)anEnumProperty);
  COM_TRY_END
  #else
    return E_NOTIMPL;
  #endif
}

STDMETHODIMP CHandler::GetNumberOfItems(UINT32 *aNumItems)
{
  COM_TRY_BEGIN
  *aNumItems = m_Database.m_Files.Size();
  return S_OK;
  COM_TRY_END
}

static void MySetFileTime(bool aDefined, FILETIME anUnixTime, 
    NWindows::NCOM::CPropVariant &aPropVariant)
{
  // FILETIME aFILETIME;
  if (aDefined)
    aPropVariant = anUnixTime;
    // NTime::UnixTimeToFileTime((time_t)anUnixTime, aFILETIME);
  else
  {
    return;
    // aFILETIME.dwHighDateTime = aFILETIME.dwLowDateTime = 0;
  }
  // aPropVariant = aFILETIME;
}

/*
inline static wchar_t GetHex(BYTE aValue)
{
  return (aValue < 10) ? ('0' + aValue) : ('A' + (aValue - 10));
}

static UString ConvertBytesToHexString(const BYTE *aData, UINT32 aSize)
{
  UString aResult;
  for (UINT32 i = 0; i < aSize; i++)
  {
    BYTE aByte = aData[i];
    aResult += GetHex(aByte >> 4);
    aResult += GetHex(aByte & 0xF);
  }
  return aResult;
}
*/

STDMETHODIMP CHandler::GetProperty(UINT32 anIndex, PROPID aPropID,  PROPVARIANT *aValue)
{
  COM_TRY_BEGIN
  NWindows::NCOM::CPropVariant aPropVariant;
  const CFileItemInfo &anItem = m_Database.m_Files[anIndex];

  switch(aPropID)
  {
    case kpidPath:
    {
      aPropVariant = NArchive::NItemName::GetOSName(anItem.Name);
      break;
    }
    case kpidIsFolder:
      aPropVariant = anItem.IsDirectory;
      break;
    case kpidSize:
      aPropVariant = anItem.UnPackSize;
      break;
    case kpidPackedSize:
    {
      {
        int aFolderIndex = m_Database.m_FileIndexToFolderIndexMap[anIndex];
        if (aFolderIndex >= 0)
        {
          const CFolderItemInfo &aFolderInfo = m_Database.m_Folders[aFolderIndex];
          if (m_Database.m_FolderStartFileIndex[aFolderIndex] == anIndex)
            aPropVariant = m_Database.GetFolderFullPackSize(aFolderIndex);
          else
            aPropVariant = UINT64(0);
        }
        else
          aPropVariant = UINT64(0);
      }
      break;
    }
    case kpidLastAccessTime:
      MySetFileTime(anItem.IsLastAccessTimeDefined, anItem.LastAccessTime, aPropVariant);
      break;
    case kpidCreationTime:
      MySetFileTime(anItem.IsCreationTimeDefined, anItem.CreationTime, aPropVariant);
      break;
    case kpidLastWriteTime:
      MySetFileTime(anItem.IsLastWriteTimeDefined, anItem.LastWriteTime, aPropVariant);
      break;
    case kpidAttributes:
      if (anItem.AreAttributesDefined)
        aPropVariant = anItem.Attributes;
      break;
    case kpidCRC:
      if (anItem.FileCRCIsDefined)
        aPropVariant = anItem.FileCRC;
      break;
    case kpidPackedSize0:
    case kpidPackedSize1:
    case kpidPackedSize2:
    case kpidPackedSize3:
    case kpidPackedSize4:
      {
        int aFolderIndex = m_Database.m_FileIndexToFolderIndexMap[anIndex];
        if (aFolderIndex >= 0)
        {
          const CFolderItemInfo &aFolderInfo = m_Database.m_Folders[aFolderIndex];
          if (m_Database.m_FolderStartFileIndex[aFolderIndex] == anIndex &&
              aFolderInfo.PackStreams.Size() > aPropID - kpidPackedSize0)
          {
            aPropVariant = m_Database.GetFolderPackStreamSize(aFolderIndex, aPropID - kpidPackedSize0);
          }
          else
            aPropVariant = UINT64(0);
        }
        else
          aPropVariant = UINT64(0);
      }
      break;
    case kpidIsAnti:
      aPropVariant = anItem.IsAnti;
      break;
  }
  aPropVariant.Detach(aValue);
  return S_OK;
  COM_TRY_END
}

STDMETHODIMP CHandler::Open(IInStream *aStream,
    const UINT64 *aMaxCheckStartPosition, IOpenArchive2CallBack *anOpenArchiveCallBack)
{
  COM_TRY_BEGIN
  m_InStream.Release();
  m_Database.Clear();
  try
  {
    CInArchive anArchive;
    RETURN_IF_NOT_S_OK(anArchive.Open(aStream, aMaxCheckStartPosition))

    RETURN_IF_NOT_S_OK(anArchive.ReadDatabase(m_Database));
    HRESULT aResult = anArchive.CheckIntegrity();
    if (aResult != S_OK)
      return E_FAIL;
    m_Database.FillFolderStartPackStream();
    m_Database.FillStartPos();
    m_Database.FillFolderStartFileIndex();
  }
  catch(...)
  {
    return S_FALSE;
  }
  m_InStream = aStream;
  return S_OK;
  COM_TRY_END
}

STDMETHODIMP CHandler::Close()
{
  COM_TRY_BEGIN
  m_InStream.Release();
  return S_OK;
  COM_TRY_END
}
