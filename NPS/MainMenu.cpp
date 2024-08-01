#include "MainMenu.h"

//======================================================================================================
void MainMenu::IsHost(bool flag)
{
	isHost = flag;
}
//======================================================================================================
void MainMenu::IsActivated(bool flag)
{
	isActivated = flag;
}
//======================================================================================================
const MainMenu::MenuItems& MainMenu::GetMenuItems() const
{
	return menuItems;
}
//======================================================================================================
void MainMenu::Show()
{
	auto MenuDivider = []()
	{
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	};

	menuItems = { false };

	if (ImGui::BeginMainMenuBar())
	{
		ImGui::PushFont(fonts["MyriadPro_Regular_14"]);
		ImGui::GetStyle().Colors[ImGuiCol_Text] = white;

		//Do not use else-if statements here otherwise the menu titles will not render correctly
		//For each menu item, the first parameter is label, second is shortcut, third is a bool for selection
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", nullptr, nullptr))
			{
				menuItems.isNewSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Load...", nullptr, nullptr))
			{
				menuItems.isLoadSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Save", nullptr, nullptr))
			{
				menuItems.isSaveSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Save As...", nullptr, nullptr))
			{
				menuItems.isSaveAsSelected = true;
			}

			MenuDivider();

			if (ImGui::MenuItem("Exit", nullptr, nullptr))
			{
				menuItems.isExitSelected = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Plate"))
		{
			if (ImGui::MenuItem("Standard Oblong (520mm x 111mm)", nullptr, nullptr))
			{
				menuItems.isStandardOblongSelected = true;
			}

			MenuDivider();

			if (ImGui::MenuItem("Short 13\" (337mm x 111mm)", nullptr, nullptr))
			{
				menuItems.isShort13InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Short 16\" (405mm x 111mm)", nullptr, nullptr))
			{
				menuItems.isShort16InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Shorter 17\" (431mm x 111mm)", nullptr, nullptr))
			{
				menuItems.isShorter17InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Shorter 18\" (455mm x 111mm)", nullptr, nullptr))
			{
				menuItems.isShorter18InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Oversize Oblong (533mm x 152mm)", nullptr, nullptr))
			{
				menuItems.isOversizeOblongSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Shaped Plate (616mm x 152mm)", nullptr, nullptr))
			{
				menuItems.isShapedPlateSelected = true;
			}

			MenuDivider();

			if (ImGui::MenuItem("Motorcycle 7x5\" (178mm x 127mm)", nullptr, nullptr))
			{
				menuItems.isMotorcycle7x5InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Motorcycle 8x6\" (203mm x 152mm)", nullptr, nullptr))
			{
				menuItems.isMotorcycle8x6InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Motorcycle 9x7\" (235mm x 178mm)", nullptr, nullptr))
			{
				menuItems.isMotorcycle9x7InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Square 11x8\" (279mm x 203mm)", nullptr, nullptr))
			{
				menuItems.isSquare11x8InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("USA Import 12x6\" (310mm x 152mm)", nullptr, nullptr))
			{
				menuItems.isUSAImport12x6InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Large Euro Square 12x8\" (310mm x 203mm)", nullptr, nullptr))
			{
				menuItems.isLargeEuroSquare12x8InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Import 13x6.5\" (330mm x 165mm)", nullptr, nullptr))
			{
				menuItems.isImport13x6Point5InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Import 13x7\" (330mm x 178mm)", nullptr, nullptr))
			{
				menuItems.isImport13x7InchSelected = true;
			}

			MenuDivider();

			if (ImGui::MenuItem("Micro Plate 9x3\" (228mm x 76mm)", nullptr, nullptr))
			{
				menuItems.isMicroPlate9x3InchSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("Micro Plate 12x3\" (310mm x 76mm)", nullptr, nullptr))
			{
				menuItems.isMicroPlate12x3InchSelected = true;
			}

			MenuDivider();

			if (ImGui::MenuItem("Font settings...", nullptr, nullptr))
			{
				menuItems.isFontSettingsSelected = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Tutorial", nullptr, nullptr))
			{
				menuItems.isTutorialSelected = true;
			}

			ImGui::Spacing();

			if (ImGui::MenuItem("About...", nullptr, nullptr))
			{
				menuItems.isAboutSelected = true;
			}

			ImGui::Spacing();
			
			if (isHost)
			{
				if (ImGui::MenuItem("Customer information...", nullptr, nullptr))
				{
					menuItems.isCustomerInformationSelected = true;
				}
			}

			else
			{
				if (isActivated)
				{
					if (ImGui::MenuItem("Check license...", nullptr, nullptr))
					{
						menuItems.isCheckLicenseSelected = true;
					}
				}

				else
				{
					if (ImGui::MenuItem("Activate program...", nullptr, nullptr))
					{
						menuItems.isActivateProgramSelected = true;
					}
				}
			}

			ImGui::EndMenu();
		}

		ImGui::PopFont();
		ImGui::EndMainMenuBar();
	}
}