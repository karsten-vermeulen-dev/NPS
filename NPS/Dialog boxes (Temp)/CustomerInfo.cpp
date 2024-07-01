#include "pch.h"
#include "NumberPlate.h"
#include "CustomerInfo.h"
#include "afxdialogex.h"
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include "Utility.h"

//================================================================================================
IMPLEMENT_DYNAMIC(CustomerInfo, CDialogEx)
//================================================================================================
BEGIN_MESSAGE_MAP(CustomerInfo, CDialogEx)
END_MESSAGE_MAP()
//================================================================================================
CustomerInfo::CustomerInfo(CWnd* parent) : CDialogEx(IDD_CUSTOMER_INFO), newKeyGenerated(false)
{
	std::ifstream file;
	file.open("CustomerInfo.csv");
	
	if (!file.is_open())
	{
		Utility::Log(Utility::Destination::WindowsMessageBox, "File not found.");
		return;
	}

	m_comboBoxCtrl.Create(CBS_SIMPLE, { 0, 0, 100, 100 }, parent, 0);
	
	std::string temp;
	std::string name;
	std::string inputStr;
	
	CustomerData customerData;
	
	getline(file, inputStr, '\n');
	std::stringstream ss(inputStr);
	
	while (!file.eof() && inputStr.size() > 2)
	{
		std::getline(ss, temp, ',');
		name = temp;

		std::getline(ss, temp, ',');
		customerData.sidCode = temp.c_str();

		std::getline(ss, temp, ',');
		customerData.email = temp.c_str();

		std::getline(ss, temp, ',');
		customerData.latestKeyUsed = temp.c_str();

		std::getline(ss, temp);
		customerData.latestExpiryDate = temp.c_str();

		m_comboBoxCtrl.AddString((LPCTSTR)(CString(L"Test")));
		m_customerMatrix.insert({ name, customerData });

		getline(file, inputStr, '\n');
		ss = std::stringstream(inputStr);
	}
}
//================================================================================================
void CustomerInfo::DoDataExchange(CDataExchange* dataExchange)
{
	CDialogEx::DoDataExchange(dataExchange);

	DDX_Control(dataExchange, IDC_COMBO_CUSTOMERS, m_comboBoxCtrl);
	DDX_Text(dataExchange, IDC_EDIT_SID, m_sidCode);
	DDX_Text(dataExchange, IDC_EDIT_EMAIL, m_email);
	DDX_Text(dataExchange, IDC_EDIT_LATEST_KEY, m_latestKeyUsed);
	//DDX_DateTimeCtrl(dataExchange, IDC_EDIT_LATEST_EXPIRY_DATE, m_latestExpiryDate);

	if (newKeyGenerated)
	{
		DDX_Text(dataExchange, IDC_EDIT_LATEST_KEY, m_newKey);
		//DDX_DateTimeCtrl(dataExchange, IDC_EDIT_LATEST_EXPIRY_DATE, m_newExpiryDate);
	}
}