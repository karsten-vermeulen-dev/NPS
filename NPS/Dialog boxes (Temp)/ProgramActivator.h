#pragma once
#include <afxdialogex.h>

class ProgramActivator : public CDialogEx
{

public:

	ProgramActivator(CWnd* parent = nullptr);
	virtual ~ProgramActivator() {}

	afx_msg void OnHelp() {}
	//afx_msg void OnActivateProgram();

	afx_msg void OnCancel();

private:

	virtual void DoDataExchange(CDataExchange* dataExchange);

	CString m_companyName;
	CString m_eMail;
	CString m_licenceKey;

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(ProgramActivator)
};