#pragma once
#include <vector>

class SyncSubMenu
{
public:
	int subMenuId;
	LPWSTR subMenuName;
	UINT uFlags = MF_STRING;

	SyncSubMenu(int _subMenuId, LPWSTR _subMenuName, UINT _uFlags)
	{
		subMenuId = _subMenuId;
		subMenuName = _subMenuName;
		uFlags = _uFlags;
	}

	SyncSubMenu(int _subMenuId, LPWSTR _subMenuName)
	{
		subMenuId = _subMenuId;
		subMenuName = _subMenuName;
	}

	//Append to a popup menu from SyncMenu
	void Append(HMENU &hMenuNew)
	{
		AppendMenu(hMenuNew, uFlags, subMenuId, subMenuName);
	}
};

class SyncMenu
{
public:
	std::vector<SyncSubMenu> subMenus;
	LPWSTR menuName;
	HMENU hSyncMenu;

	SyncMenu()
	{

	}
	SyncMenu(LPWSTR _menuName)
	{
		menuName = _menuName;
	}

	SyncMenu(LPWSTR _menuName, std::vector<SyncSubMenu> _subMenus)
	{
		subMenus = _subMenus;
		menuName = _menuName;
	}

	//Variatic template to allow easy construction 
	template<typename...Args>
	SyncMenu(LPWSTR _menuName, Args...args)
	{
		//Unpack args into vector
		std::vector<SyncSubMenu> _subMenu = { args... };
		subMenus = _subMenu;
		menuName = _menuName;
	}

	SyncMenu(LPWSTR _menuName, SyncSubMenu _subMenu)
	{
		subMenus.push_back(_subMenu);
		menuName = _menuName;
	}

	void AddMenu(SyncSubMenu subMenu)
	{
		subMenus.push_back(subMenu);
	}

	void Create(HMENU &hMenu)
	{
		hSyncMenu = CreateMenu();

		//Create popup menu
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (unsigned int)hSyncMenu, menuName);

		//If submenu is not empty, add SyncSubMenu submenus to popup menu
		if (!subMenus.empty())
		{
			for each(auto MenuItem in subMenus)
			{
				//For each menu item, append it to the popup menu
				MenuItem.Append(hSyncMenu);
			}			
		}

	}

	void Destroy(HWND hWnd)
	{
		CMenu *cSyncMenu = CMenu::FromHandle(GetMenu(hWnd));
		if (cSyncMenu != NULL)
		{
			//Case hSyncMenu as UINT to get position
			cSyncMenu->RemoveMenu((UINT)hSyncMenu, MF_BYCOMMAND);
		}
	}
};