#pragma once

#include "Plate.h"
#include "UserInterface.h"

class FeedbackPanel : public UserInterface
{

public:

	void SetLegality(bool flag);
	void SetPlateData(const Plate::PlateData& plateData);

	virtual void Show() override;
	virtual void Reset() override {}

private:

	bool isLegal{ false };
	Plate::PlateData plateData;

};