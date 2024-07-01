#pragma once
#include <afxdialogex.h>

class LicenceKeyGenerator : public CDialogEx
{
public:

	LicenceKeyGenerator(CWnd* parent = nullptr);
	virtual ~LicenceKeyGenerator() {}

	afx_msg void OnGetKey();
	afx_msg void OnCheckKey();
	afx_msg void OnHelp() {}

private:

	virtual void DoDataExchange(CDataExchange* dataExchange);

	// Taken from: https://forums.codeguru.com/showthread.php?433599-How-to-add-a-month-in-CTime-object
	int LicenceKeyGenerator::GetDaysOfMonth(const CTime& time);

	int m_periodMonths;
	int m_endOfMonth;
	int m_siteCode;
	int m_licenceKey;
	bool m_validLicenceKey;

	CTime m_startDate;
	CString m_companyName;
	CSpinButtonCtrl m_spin;

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(LicenceKeyGenerator)
};