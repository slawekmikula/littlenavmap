/*****************************************************************************
* Copyright 2015-2020 Alexander Barthel alex@littlenavmap.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef LITTLENAVMAP_USERICONDELEGATE_H
#define LITTLENAVMAP_USERICONDELEGATE_H

#include <QStyledItemDelegate>

class ColumnList;
class SymbolPainter;
class UserdataIcons;

/*
 * Paints userdata icons into the "type" cell of the search result table view.
 */
class UserIconDelegate :
  public QStyledItemDelegate
{
  Q_OBJECT

public:
  UserIconDelegate(const ColumnList *columns, UserdataIcons *userdataIcons);
  virtual ~UserIconDelegate();

private:
  const ColumnList *cols;
  SymbolPainter *symbolPainter;

  virtual void paint(QPainter *painter, const QStyleOptionViewItem& option,
                     const QModelIndex& index) const override;

  UserdataIcons *icons;
};

#endif // LITTLENAVMAP_USERICONDELEGATE_H
