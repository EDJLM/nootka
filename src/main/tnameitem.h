/***************************************************************************
 *   Copyright (C) 2017-2018 by Tomasz Bojczuk                             *
 *   seelook@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#ifndef TNAMEOBJECT_H
#define TNAMEOBJECT_H


#include <music/tnote.h>
#include <QtQuick/qquickitem.h>


#define NOTENAME TnameItem::instance()

/**
 * C++ logic of NoteName.qml control
 */
class TnameItem : public QQuickItem
{

  Q_OBJECT

  Q_PROPERTY(Tnote note READ note WRITE setNote NOTIFY noteChanged)
  Q_PROPERTY(int step READ step WRITE setStep NOTIFY stepChanged)
  Q_PROPERTY(int alter READ alter WRITE setAlter NOTIFY alterChanged)
  Q_PROPERTY(int octave READ octave WRITE setOctave NOTIFY octaveChanged)
  Q_PROPERTY(QString nameText READ nameText NOTIFY nameTextChanged)
  Q_PROPERTY(int nameStyle READ nameStyle WRITE setNameStyle NOTIFY nameStyleChanged)
  Q_PROPERTY(int buttonNameStyle READ buttonNameStyle WRITE setButtonNameStyle NOTIFY buttonNameStyleChanged)
  Q_PROPERTY(QString appendix READ appendix NOTIFY appendixChanged)
  Q_PROPERTY(QColor bgColor READ bgColor NOTIFY bgColorChanged)
  Q_PROPERTY(bool disabled READ disabled WRITE setDisabled NOTIFY disabledChanged)
  Q_PROPERTY(QColor markColor READ markColor WRITE setMarkColor NOTIFY markColorChanged)


public:
  TnameItem (QQuickItem* parent = nullptr);
  ~TnameItem() override;

  static TnameItem* instance() { return m_instance; }

  Tnote note() const { return m_note; }
  void setNote(const Tnote& n);

  int step() const { return static_cast<int>(m_note.note()); }
  void setStep (int st);

  int alter() const { return static_cast<int>(m_note.alter()); }
  void setAlter(int alt);

  int octave() const { return static_cast<int>(m_note.octave()); }
  void setOctave(int oct);

  int nameStyle() const { return static_cast<int>(m_nameStyle); }
  void setNameStyle(int nStyle) { setNameStyle(static_cast<Tnote::EnameStyle>(nStyle)); }
  void setNameStyle(Tnote::EnameStyle style);

  int buttonNameStyle() const { return static_cast<int>(m_buttonNameStyle); }
  void setButtonNameStyle(Tnote::EnameStyle style);
  void setButtonNameStyle(int nStyle) { setButtonNameStyle(static_cast<Tnote::EnameStyle>(nStyle)); }

  bool disabled() const { return m_disabled; }
  void setDisabled(bool dis);

  QColor markColor() const { return m_outlineColor; }
  void setMarkColor(const QColor& outColor);

  QString nameText() const;

      /**
       * This is additional text displayed in exam mode: '?' (question mark) and string number (if any)
       */
  QString appendix() const { return m_appendix; }

  QColor bgColor() const { return m_bgColor; }

  Q_INVOKABLE QString octaveName(int oNr) const;
  Q_INVOKABLE QString octavesLink() const;
  Q_INVOKABLE QString noteButtonText(int noteNr, int nStyle = -1);

  void askQuestion(const Tnote& note, Tnote::EnameStyle questStyle, quint8 strNr = 0);

  void prepareAnswer(Tnote::EnameStyle answStyle);

  void forceAccidental(char accid);

signals:
  void noteChanged();
  void stepChanged();
  void alterChanged();
  void octaveChanged();
  void nameTextChanged();
  void nameStyleChanged();
  void appendixChanged();
  void bgColorChanged();
  void buttonNameStyleChanged();
  void disabledChanged();
  void noteButtonClicked();
  void markColorChanged();

private:
  void changeNameBgColor(const QColor& c) { m_bgColor = c;  emit bgColorChanged(); }

private:
  Tnote                   m_note;
  Tnote::EnameStyle       m_nameStyle, m_buttonNameStyle;
  QString                 m_appendix;
  QColor                  m_bgColor, m_outlineColor;
  bool                    m_disabled = false;

  static TnameItem       *m_instance;
};

#endif // TNAMEOBJECT_H
