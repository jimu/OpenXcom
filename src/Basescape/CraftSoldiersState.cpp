/*
 * Copyright 2010 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "CraftSoldiersState.h"
#include <string>
#include <sstream>
#include "../Engine/Game.h"
#include "../Resource/ResourcePack.h"
#include "../Engine/Language.h"
#include "../Engine/Font.h"
#include "../Engine/Palette.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Base.h"
#include "../Savegame/Soldier.h"
#include "../Savegame/Craft.h"
#include "../Ruleset/RuleCraft.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Craft Soldiers screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 * @param craft ID of the selected craft.
 */
CraftSoldiersState::CraftSoldiersState(Game *game, Base *base, unsigned int craft) : State(game), _base(base), _craft(craft)
{
	// Create objects
	_window = new Window(this, 320, 200, 0, 0);
	_btnOk = new TextButton(288, 16, 16, 176);
	_txtTitle = new Text(300, 16, 16, 7);
	_txtName = new Text(114, 9, 16, 32);
	_txtRank = new Text(102, 9, 130, 32);
	_txtCraft = new Text(82, 9, 222, 32);
	_txtAvailable = new Text(110, 9, 16, 24);
	_txtUsed = new Text(110, 9, 130, 24);
	_lstSoldiers = new TextList(288, 128, 8, 40);

	// Set palette
	_game->setPalette(_game->getResourcePack()->getPalette("BACKPALS.DAT")->getColors(Palette::blockOffset(2)), Palette::backPos, 16);

	add(_window);
	add(_btnOk);
	add(_txtTitle);
	add(_txtName);
	add(_txtRank);
	add(_txtCraft);
	add(_txtAvailable);
	add(_txtUsed);
	add(_lstSoldiers);

	// Set up objects
	_window->setColor(Palette::blockOffset(15)+6);
	_window->setBackground(_game->getResourcePack()->getSurface("BACK02.SCR"));

	_btnOk->setColor(Palette::blockOffset(13)+10);
	_btnOk->setText(_game->getLanguage()->getString("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&CraftSoldiersState::btnOkClick);

	_txtTitle->setColor(Palette::blockOffset(15)+6);
	_txtTitle->setBig();
	Craft *c = _base->getCrafts()->at(_craft);
	std::wstring s;
	s = _game->getLanguage()->getString("STR_SELECT_SQUAD_FOR") + c->getName(_game->getLanguage());
	_txtTitle->setText(s);

	_txtName->setColor(Palette::blockOffset(15)+6);
	_txtName->setText(_game->getLanguage()->getString("STR_NAME_UC"));

	_txtRank->setColor(Palette::blockOffset(15)+6);
	_txtRank->setText(_game->getLanguage()->getString("STR_RANK"));

	_txtCraft->setColor(Palette::blockOffset(15)+6);
	_txtCraft->setText(_game->getLanguage()->getString("STR_CRAFT"));

	_txtAvailable->setColor(Palette::blockOffset(15)+6);
	_txtAvailable->setSecondaryColor(Palette::blockOffset(13));
	std::wstringstream ss;
	ss << _game->getLanguage()->getString("STR_SPACE_AVAILABLE") << L'\x01' << c->getRules()->getSoldiers() - c->getNumSoldiers();
	_txtAvailable->setText(ss.str());

	_txtUsed->setColor(Palette::blockOffset(15)+6);
	_txtUsed->setSecondaryColor(Palette::blockOffset(13));
	std::wstringstream ss2;
	ss2 << _game->getLanguage()->getString("STR_SPACE_USED") << L'\x01' << c->getNumSoldiers();
	_txtUsed->setText(ss2.str());

	_lstSoldiers->setColor(Palette::blockOffset(13)+10);
	_lstSoldiers->setArrowColor(Palette::blockOffset(15)+6);
	_lstSoldiers->setColumns(3, 114, 92, 74);
	_lstSoldiers->setSelectable(true);
	_lstSoldiers->setBackground(_window);
	_lstSoldiers->setMargin(8);
	_lstSoldiers->onMouseClick((ActionHandler)&CraftSoldiersState::lstSoldiersClick);

	int row = 0;
	for (std::vector<Soldier*>::iterator i = _base->getSoldiers()->begin(); i != _base->getSoldiers()->end(); ++i)
	{
		std::wstring s;
		if ((*i)->getCraft() == 0)
		{
			s = _game->getLanguage()->getString("STR_NONE_UC");
		}
		else
		{
			s = (*i)->getCraft()->getName(_game->getLanguage());
		}
		_lstSoldiers->addRow(3, (*i)->getName().c_str(), _game->getLanguage()->getString((*i)->getRankString()).c_str(), s.c_str());

		Uint8 color;
		if ((*i)->getCraft() == c)
		{
			color = Palette::blockOffset(13);
		}
		else if ((*i)->getCraft() != 0)
		{
			color = Palette::blockOffset(15)+6;
		}
		else
		{
			color = Palette::blockOffset(13)+10;
		}
		_lstSoldiers->setRowColor(row, color);
		row++;
	}
}

/**
 *
 */
CraftSoldiersState::~CraftSoldiersState()
{
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void CraftSoldiersState::btnOkClick(Action *action)
{
	_game->popState();
}

/**
 * Shows the selected soldier's info.
 * @param action Pointer to an action.
 */
void CraftSoldiersState::lstSoldiersClick(Action *action)
{
	int row = _lstSoldiers->getSelectedRow();
	Craft *c = _base->getCrafts()->at(_craft);
	Soldier *s = _base->getSoldiers()->at(_lstSoldiers->getSelectedRow());
	Uint8 color = Palette::blockOffset(13)+10;
	if (s->getCraft() == c)
	{
		s->setCraft(0);
		_lstSoldiers->setCellText(row, 2, _game->getLanguage()->getString("STR_NONE_UC"));
		color = Palette::blockOffset(13)+10;
	}
	else if (c->getRules()->getSoldiers() - c->getNumSoldiers() > 0)
	{
		s->setCraft(c);
		_lstSoldiers->setCellText(row, 2, c->getName(_game->getLanguage()));
		color = Palette::blockOffset(13);
	}
	_lstSoldiers->setRowColor(row, color);

	std::wstringstream ss;
	ss << _game->getLanguage()->getString("STR_SPACE_AVAILABLE") << L'\x01' << c->getRules()->getSoldiers() - c->getNumSoldiers();
	_txtAvailable->setText(ss.str());
	std::wstringstream ss2;
	ss2 << _game->getLanguage()->getString("STR_SPACE_USED") << L'\x01' << c->getNumSoldiers();
	_txtUsed->setText(ss2.str());
}

}
