#include "pch.h"
#include "NumberPlate.h"
#include "ProgramActivator.h"
#include "afxdialogex.h"
#include "Utility.h"

//================================================================================================
IMPLEMENT_DYNAMIC(ProgramActivator, CDialogEx)
//================================================================================================
BEGIN_MESSAGE_MAP(ProgramActivator, CDialogEx)
    ON_BN_CLICKED(IDCANCEL, &ProgramActivator::OnCancel)
	ON_BN_CLICKED(IDC_HELP_LKG, &ProgramActivator::OnHelp)
	//ON_BN_CLICKED(ID_ACTIVATE_PROGRAM, &ProgramActivator::OnActivateProgram)
END_MESSAGE_MAP()
//================================================================================================
ProgramActivator::ProgramActivator(CWnd* parent) : CDialogEx(IDD_PROGRAM_ACTIVATOR),
	m_companyName("Test Company Name"),
	m_eMail("Test Company Email") {}
//================================================================================================
void ProgramActivator::DoDataExchange(CDataExchange* dataExchange)
{
	CDialogEx::DoDataExchange(dataExchange);

	DDX_Text(dataExchange, IDC_EDIT_COMPANY_NAME, m_companyName);
	DDX_Text(dataExchange, IDC_EDIT_EMAIL, m_eMail);
	DDX_Text(dataExchange, IDC_EDIT_LICENCE_KEY, m_licenceKey);
}
//================================================================================================
//void ProgramActivator::OnActivateProgram()
//{
//    // Get the data from the dialog to variables
//    UpdateData();
//
//    // Based on: KeyzyClientLibrary0.0.19 documentation examples
//
//    // Fill ProductData struct to identify specific product
//    // This will be most likely hardcoded into the program
//    Keyzy::ProductData data;
//    data._appId = "310EjD4a";
//    data._apiKey = "WE57TjtdAKixb9SXC1ki1lvs1oj9xWf4OAoSzhDE";
//    data._productCode = "310a6ba0-4e07-11ec-8138-4b4cd77f1d1c";
//
//    // Get an activator object
//    std::unique_ptr<Keyzy::KeyzyLicenseActivator> pLicenseActivator =
//        std::make_unique<Keyzy::KeyzyLicenseActivator>(data);
//
//    // Get a validator object
//    std::shared_ptr<Keyzy::KeyzyLicenseValidator> pLicenseValidator =
//        pLicenseActivator->getLicenseValidator();
//
//    // Read the serial number given by the user
//    const std::string serialNumber = CT2A(m_licenceKey);
//
//    // Useful variables
//    Keyzy::LicenseStatus status;
//    CString message;
//
//    // Check if the licence is valid (activated)
//    status = pLicenseValidator->validateOnline();
//    if (status != Keyzy::LicenseStatus::VALID) 
//    {
//        // If not valid (activated) - try to activate
//        status = pLicenseActivator->activateOnline(serialNumber);
//    }
//
//    // Format and display the message - could be adjustem to the needs
//    message.Format(errorStatuses(status).c_str());
//    message += "\nLicensee's Name: ";
//    message += pLicenseValidator->getLicenseeName().c_str();
//    message += "\nLicensee's Email: ";
//    message += pLicenseValidator->getLicenseeEmail().c_str();
//    message += "\nVersion: ";
//    message += pLicenseValidator->getVersion().c_str();
//    message += "\nSerial No: ";
//    message += serialNumber.c_str();
//    AfxMessageBox(message);
//
//    // Close the dialog if the licence is valid
//    if (status == Keyzy::LicenseStatus::VALID)
//        CDialogEx::OnOK();
//}
//================================================================================================
void ProgramActivator::OnCancel()
{
    CDialogEx::OnCancel();
}