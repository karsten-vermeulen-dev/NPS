#include "CustomPlate.h"
#include "NumberPlate.h"

//================================================================================================
IMPLEMENT_DYNCREATE(CustomPlate, CDialogEx)
//================================================================================================
BEGIN_MESSAGE_MAP(CustomPlate, CDialogEx)
END_MESSAGE_MAP()
//================================================================================================
CustomPlate::CustomPlate() : CDialogEx(IDD_CUSTOM_PLATE),
	m_isCar(false), m_hasCarFont(false), m_isTwoLineRegistration(false)
{
	m_width = 0;
	m_height = 0;
}
//================================================================================================
int CustomPlate::GetWidth() const
{
	return m_width;
}
//================================================================================================
int CustomPlate::GetHeight() const
{
	return m_height;
}
//================================================================================================
bool CustomPlate::IsCar() const
{
	return !m_isCar;
}
//================================================================================================
bool CustomPlate::IsTwoLineRegistration() const
{
	return m_isTwoLineRegistration;
}
//================================================================================================
bool CustomPlate::HasCarFont() const
{
	return !m_hasCarFont;
}
//================================================================================================
void CustomPlate::DoDataExchange(CDataExchange* dataExchange)
{
	CDialogEx::DoDataExchange(dataExchange);

	DDX_Text(dataExchange, IDC_PLATE_WIDTH, m_width);
	DDX_Text(dataExchange, IDC_PLATE_HEIGHT, m_height);

	DDX_Radio(dataExchange, IDC_RADIO_CAR,
		reinterpret_cast<int&>(m_isCar));
	DDX_Radio(dataExchange, IDC_RADIO_CAR_FONT,
		reinterpret_cast<int&>(m_hasCarFont));
	DDX_Check(dataExchange, IDC_CHECK_TWO_LINE,
		reinterpret_cast<int&>(m_isTwoLineRegistration));
}