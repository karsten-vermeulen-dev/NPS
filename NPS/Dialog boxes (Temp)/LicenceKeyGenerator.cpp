#include "pch.h"
#include "NumberPlate.h"
#include "LicenceKeyGenerator.h"
#include "afxdialogex.h"
#include <random>
#include <string>

//================================================================================================
IMPLEMENT_DYNAMIC(LicenceKeyGenerator, CDialogEx)
//================================================================================================
BEGIN_MESSAGE_MAP(LicenceKeyGenerator, CDialogEx)
	ON_BN_CLICKED(ID_GET_KEY, &LicenceKeyGenerator::OnGetKey)
	ON_BN_CLICKED(IDC_CHECK_KEY, &LicenceKeyGenerator::OnCheckKey)
	ON_BN_CLICKED(IDC_HELP_LKG, &LicenceKeyGenerator::OnHelp)
	ON_BN_CLICKED(IDCLOSE, &LicenceKeyGenerator::OnClose)
END_MESSAGE_MAP()
//================================================================================================
LicenceKeyGenerator::LicenceKeyGenerator(CWnd* parent) : CDialogEx(IDD_KEY_GENERATOR),
m_companyName(""), m_startDate(CTime::GetCurrentTime()),
m_periodMonths(1), m_endOfMonth(false),
m_siteCode(0), m_licenceKey(0),
m_validLicenceKey(false) {}
//================================================================================================
void LicenceKeyGenerator::DoDataExchange(CDataExchange* dataExchange)
{
	CDialogEx::DoDataExchange(dataExchange);

	DDX_Text(dataExchange, IDC_EDIT_COMPANY_NAME, m_companyName);
	DDX_DateTimeCtrl(dataExchange, IDC_DATE, m_startDate);
	DDX_Text(dataExchange, IDC_EDIT_PERIOD_MONTHS, m_periodMonths);
	DDX_Control(dataExchange, IDC_SPIN, m_spin);
	DDX_Check(dataExchange, IDC_CHECK_END_OF_MONTH, m_endOfMonth);

	DDX_Text(dataExchange, IDC_EDIT_SITE_CODE, m_siteCode);
	if (m_validLicenceKey)
	{
		DDX_Text(dataExchange, IDC_EDIT_LICENCE_KEY, m_licenceKey);
	}

	m_spin.SetRange(1, 120);
}
//================================================================================================
int LicenceKeyGenerator::GetDaysOfMonth(const CTime& time)
{
	CTime tNext(time.GetYear(), time.GetMonth(), 1, 0, 0, 0);
	tNext += CTimeSpan(31, 1, 0, 0);
	return 32 - tNext.GetDay();
}
//================================================================================================
void LicenceKeyGenerator::OnGetKey()
{
	if (m_validLicenceKey)
	{
		return;
	}

	OnCheckKey();

	// Get the licence key
	std::default_random_engine generator;
	generator.seed(m_siteCode);
	std::uniform_int_distribution<int> distribution(100000000, 999999999); // 9 - digit
	m_validLicenceKey = true;
	m_licenceKey = distribution(generator);
	UpdateData(false);
}
//================================================================================================
void LicenceKeyGenerator::OnCheckKey()
{
	if (m_validLicenceKey)
	{
		return;
	}

	// Update the data based on the dialog inputs
	UpdateData();

	// Based on: https://forums.codeguru.com/showthread.php?433599-How-to-add-a-month-in-CTime-object

	// Show the expire date and time left on the licence =================

	// Get data from the start date and add number of months specified
	int year = m_startDate.GetYear();
	int month = m_startDate.GetMonth() + m_periodMonths;
	int day = m_startDate.GetDay();

	while (month > 12)
	{
		month -= 12;
		year++;
	}

	// Get the date with the specified month
	CTime tNext(year, month, 1, 0, 0, 0);

	// Check the number of days in this month
	int nDays = GetDaysOfMonth(tNext);

	// Depending on the settings, adjust the day
	int newDay = nDays;
	if (newDay > day && !m_endOfMonth)
	{
		newDay = day;
	}

	// Construct the expiry date
	CTime expiryDate(year, month, newDay,
		m_startDate.GetHour(), m_startDate.GetMinute(), m_startDate.GetSecond());

	// Get the difference
	CTimeSpan timeLeft(expiryDate - CTime::GetCurrentTime());

	// Send the message
	CString message;
	message.Format(_T("Expire Date: %d:%d:%d\nLeft %d days and %d hours"),
		expiryDate.GetDay(), expiryDate.GetMonth(), expiryDate.GetYear(), timeLeft.GetDays(), timeLeft.GetHours());
	AfxMessageBox(message);
}