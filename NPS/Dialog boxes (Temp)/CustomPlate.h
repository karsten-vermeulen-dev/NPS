#pragma once
#include <afxdialogex.h>

class CustomPlate : public CDialogEx
{

public:

	CustomPlate();
	virtual ~CustomPlate() {}

	bool IsCar() const;
	bool HasCarFont() const;
	int GetWidth() const;
	int GetHeight() const;
	bool IsTwoLineRegistration() const;

private:

	virtual void DoDataExchange(CDataExchange* dataExchange);
	
	int m_width;
	int m_height;
	bool m_isCar;
	bool m_hasCarFont;
	bool m_isTwoLineRegistration;

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CustomPlate)

};