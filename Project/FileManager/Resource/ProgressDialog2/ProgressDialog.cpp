// ProgressDialog.cpp

#include "StdAfx.h"
#include "resource.h"
#include "ProgressDialog.h"
#include "Common/IntToString.h"
#include "Common/IntToString.h"

using namespace NWindows;

static const UINT_PTR kTimerID = 3;
static const UINT kTimerElapse = 50;

#ifdef LANG        
#include "../../LangUtils.h"
#endif

#ifdef LANG        
static CIDLangPair kIDLangPairs[] = 
{
  { IDCANCEL, 0x02000711 }
};
#endif

#ifndef _SFX
CProgressDialog::~CProgressDialog()
{
  AddToTitle(TEXT(""));
}
void CProgressDialog::AddToTitle(LPCTSTR string)
{
  if (MainWindow != 0)
    ::SetWindowText(MainWindow, string + CSysString(MainTitle));
}
#endif



bool CProgressDialog::OnInit() 
{
  _range = UINT64(-1);
  _prevPercentValue = UINT32(-1);
  _prevElapsedSec = UINT32(-1);
  _prevRemainingSec = UINT32(-1);
  _prevSpeed = UINT32(-1);
  _prevMode = kSpeedBytes;
  _pevTime = ::GetTickCount();
  _elapsedTime = 0;
  _foreground = true;

  #ifdef LANG        
  // LangSetWindowText(HWND(*this), 0x02000C00);
  LangSetDlgItemsText(HWND(*this), kIDLangPairs, sizeof(kIDLangPairs) / sizeof(kIDLangPairs[0]));
  #endif

  m_ProgressBar.Attach(GetItem(IDC_PROGRESS1));
  _timer = SetTimer(kTimerID, kTimerElapse);
  _dialogCreatedEvent.Set();
  SetText(_title);
  SetPauseText();
  SetPriorityText();
	return CModalDialog::OnInit();
}

void CProgressDialog::OnCancel() 
{
  ProgressSynch.SetStopped(true);
}

static void ConvertSizeToString(UINT64 value, TCHAR *string)
{
  if (value < (UINT64(10000) <<  0))
  {
    ConvertUINT64ToString(value, string);
    _tcscat(string, TEXT(" B"));
    return;
  }
  if (value < (UINT64(10000) <<  10))
  {
    ConvertUINT64ToString((value >> 10), string);
    _tcscat(string, TEXT(" KB"));
    return;
  }
  if (value < (UINT64(10000) <<  20))
  {
    ConvertUINT64ToString((value >> 20), string);
    _tcscat(string, TEXT(" MB"));
    return;
  }
  ConvertUINT64ToString((value >> 30), string);
  _tcscat(string, TEXT(" GB"));
  return;
}

void CProgressDialog::SetRange(UINT64 range)
{
  _range = range;
  _peviousPos = _UI64_MAX;
  _converter.Init(range);
  m_ProgressBar.SetRange32(0 , _converter.Count(range)); // Test it for 100%

  TCHAR s[32];
  ConvertSizeToString(_range, s);
  SetItemText(IDC_PROGRESS_SPEED_TOTAL_VALUE, s);
}

void CProgressDialog::SetPos(UINT64 pos)
{
  bool redraw = true;
  if (pos < _range && pos > _peviousPos)
  {
    UINT32 posDelta = pos - _peviousPos;
    if (posDelta < (_range >> 10))
      redraw = false;
  }
  if(redraw)
  {
    m_ProgressBar.SetPos(_converter.Count(pos));  // Test it for 100%
    _peviousPos = pos;
  }
}

static void GetTimeString(UINT32 timeValue, TCHAR *string)
{
  wsprintf(string, TEXT("%02d:%02d:%02d"), timeValue / 3600,
      (timeValue / 60) % 60, timeValue % 60);
}

bool CProgressDialog::OnTimer(WPARAM timerID, LPARAM callback)
{
  if (ProgressSynch.GetPaused())
    return true;
  UINT64 total, completed;
  ProgressSynch.GetProgress(total, completed);

  UINT32 curTime = ::GetTickCount();

  if (total != _range)
    SetRange(total);
  SetPos(completed);

  _elapsedTime += (curTime - _pevTime);
  _pevTime = curTime;

  UINT32 elapsedSec = _elapsedTime / 1000;

  bool elapsedChanged = false;
  if (elapsedSec != _prevElapsedSec)
  {
    TCHAR s[40];
    GetTimeString(elapsedSec, s);
    SetItemText(IDC_PROGRESS_ELAPSED_VALUE, s);
    _prevElapsedSec = elapsedSec;
    elapsedChanged = true;
  }

  if (completed != 0 && elapsedChanged)
  {
    UINT32 remainingTime = 0;
    if (completed < total)
      remainingTime = _elapsedTime * (total - completed)  / completed;
    UINT32 remainingSec = remainingTime / 1000;
    if (remainingSec != _prevRemainingSec)
    {
      TCHAR s[40];
      GetTimeString(remainingSec, s);
      SetItemText(IDC_PROGRESS_REMAINING_VALUE, s);
      _prevRemainingSec = remainingSec;
    }
    // if (elapsedChanged)
    {
      UINT32 speedB = (completed * 1000) / _elapsedTime;
      UINT32 speedKB = speedB / 1024;
      UINT32 speedMB = speedKB / 1024;
      const UINT32 kLimit1 = 10;
      TCHAR s[40];
      bool needRedraw = false;
      if (speedMB >= kLimit1)
      {
        if (_prevMode != kSpeedMBytes || speedMB != _prevSpeed)
        {
          ConvertUINT64ToString(speedMB, s);
          lstrcat(s, TEXT(" MB/s"));
          _prevMode = kSpeedMBytes;
          _prevSpeed = speedMB;
          needRedraw = true;
        }
      }
      else if (speedKB >= kLimit1)
      {
        if (_prevMode != kSpeedKBytes || speedKB != _prevSpeed)
        {
          ConvertUINT64ToString(speedKB, s);
          lstrcat(s, TEXT(" KB/s"));
          _prevMode = kSpeedKBytes;
          _prevSpeed = speedKB;
          needRedraw = true;
        }
      }
      else 
      {
        if (_prevMode != kSpeedBytes || speedB != _prevSpeed)
        {
          ConvertUINT64ToString(speedB, s);
          lstrcat(s, TEXT(" B/s"));
          _prevMode = kSpeedBytes;
          _prevSpeed = speedB;
          needRedraw = true;
        }
      }
      if (needRedraw)
        SetItemText(IDC_PROGRESS_SPEED_VALUE, s);
    }
  }

  if (total == 0)
    total = 1;
  UINT32 percentValue = (UINT32)(completed * 100 / total);
  if (percentValue != _prevPercentValue) 
  {
    TCHAR string[64];
    ConvertUINT64ToString(percentValue, string);
    CSysString title = string;
    title += TEXT("% ");
    if (!_foreground)
      title += TEXT("Background ");
    SetText(title + _title);
    #ifndef _SFX
    AddToTitle(title + MainAddTitle);
    #endif
    _prevPercentValue = percentValue;
  }
  return true;
}


////////////////////
// CU64ToI32Converter

static const UINT64 kMaxIntValue = 0x7FFFFFFF;

void CU64ToI32Converter::Init(UINT64 range)
{
  _numShiftBits = 0;
  while(range > kMaxIntValue)
  {
    range >>= 1;
    _numShiftBits++;
  }
}

int CU64ToI32Converter::Count(UINT64 aValue)
{
  return int(aValue >> _numShiftBits);
}

const UINT CProgressDialog::kCloseMessage = WM_USER + 1;

bool CProgressDialog::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
  switch(message)
  {
    case kCloseMessage:
    {
      KillTimer(_timer);
      _timer = 0;
      End(0);
      return true;
    }
    case WM_SETTEXT:
    {
      if (_timer == 0)
        return true;
    }
  }
  return CModalDialog::OnMessage(message, wParam, lParam);
}

void CProgressDialog::SetPauseText()
{
  SetItemText(IDC_BUTTON_PAUSE, ProgressSynch.GetPaused() ? 
    TEXT("&Continue") : 
    TEXT("&Pause"));
  SetText(CSysString(TEXT("Paused ")) + _title);
}

void CProgressDialog::OnPauseButton()
{
  bool paused = !ProgressSynch.GetPaused();
  ProgressSynch.SetPaused(paused);
  UINT32 curTime = ::GetTickCount();
  if (paused)
    _elapsedTime += (curTime - _pevTime);
  _pevTime = curTime;
  SetPauseText();
}

void CProgressDialog::SetPriorityText()
{
  SetItemText(IDC_BUTTON_PROGRESS_PRIORITY, _foreground ? 
      TEXT("&Background") : 
      TEXT("&Foreground"));
}

void CProgressDialog::OnPriorityButton()
{
  _foreground = !_foreground;
  SetPriorityClass(GetCurrentProcess(), _foreground ?
    NORMAL_PRIORITY_CLASS: IDLE_PRIORITY_CLASS);
  SetPriorityText();
}

bool CProgressDialog::OnButtonClicked(int buttonID, HWND buttonHWND) 
{ 
  switch(buttonID)
  {
    case IDCANCEL:
    {
      bool paused = ProgressSynch.GetPaused();;
      // ProgressSynch.SetPaused(true);
      if (!paused)
        OnPauseButton();
      int res = ::MessageBox(HWND(*this), TEXT("Are you sure you want to cancel?"), 
          _title, MB_YESNOCANCEL);
      // ProgressSynch.SetPaused(paused);
      if (!paused)
        OnPauseButton();
      if (res == IDCANCEL || res == IDNO)
        return true;
      break;
    }
    case IDC_BUTTON_PAUSE:
      OnPauseButton();
      return true;
    case IDC_BUTTON_PROGRESS_PRIORITY:
    {
      OnPriorityButton();
      return true;
    }
  }
  return CModalDialog::OnButtonClicked(buttonID, buttonHWND);
}
