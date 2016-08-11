#include "3dmol.h"

#include "3dmoldialog.h"

#include <QtWidgets/QAction>

#include <QtCore/QStringList>

namespace Avogadro {
namespace QtPlugins {

ThreeDMol::ThreeDMol(QObject *parent_) :
  Avogadro::QtGui::ExtensionPlugin(parent_),
  m_action(new QAction(this)),
  m_dialog(NULL),
  m_molecule(NULL)
{
  m_action->setEnabled(true);
  m_action->setText("&3DMol HTML Snippet...");
  connect(m_action, SIGNAL(triggered()), SLOT(showDialog()));
}

ThreeDMol::~ThreeDMol()
{
}

QString ThreeDMol::description() const
{
  return tr("ThreeDMol HTML Block.");
}

QList<QAction *> ThreeDMol::actions() const
{
  return QList<QAction*>() << m_action;
}

QStringList ThreeDMol::menuPath(QAction *) const
{
  return QStringList() << tr("&Extensions");
}

void ThreeDMol::setMolecule(QtGui::Molecule *mol)
{
  if (mol == m_molecule)
    return;

  m_molecule = mol;
  if (m_dialog)
    m_dialog->setMolecule(m_molecule);
}

void ThreeDMol::showDialog()
{
  if (!m_dialog) {
    m_dialog = new ThreeDMolDialog(
          m_molecule, qobject_cast<QWidget*>(this->parent()));
  }
  m_dialog->show();
}

} // namespace QtPlugins
} // namespace Avogadro
