#include "switchworkspacedialog.h"
#include "ui_switchworkspacedialog.h"

namespace ExtensionSystem {

QString SwitchWorkspaceDialog::WorkspacesListKey = "WorkspacesList";
QString SwitchWorkspaceDialog::CurrentWorkspaceKey = "CurrentWorkspace";
QString SwitchWorkspaceDialog::SkipChooseWorkspaceKey = "SkipChooseWorkspace";

SwitchWorkspaceDialog::SwitchWorkspaceDialog(SettingsPtr settings) :
    QDialog(0),
    ui(new Ui::SwitchWorkspaceDialog),
    settings_(settings)
{
    ui->setupUi(this);
    QStringList list = settings_->value(WorkspacesListKey, QStringList() << QDir::homePath()+"/Kumir/").toStringList();
    for (int i=0; i<list.size(); i++) {
        list[i] = QDir::toNativeSeparators(list[i]);
    }
    ui->comboBox->addItems(list);
    ui->checkBox->setChecked(settings_->value(SkipChooseWorkspaceKey, false).toBool());
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(handleBrowseClicked()));
    connect(this, SIGNAL(accepted()), this, SLOT(handleAccepted()));
}

void SwitchWorkspaceDialog::setCurrentWorkspace(const QString &path)
{
    const QString nativeName = QDir::toNativeSeparators(path.endsWith("/")? path : path + "/");
    int index = -1;
    for (int i=0; i<ui->comboBox->count(); i++) {
        if (ui->comboBox->itemText(i).trimmed()==nativeName) {
            index = i;
            break;
        }
    }
    if (index!=-1) {
        ui->comboBox->setCurrentIndex(index);
    }
    else {
        ui->comboBox->insertItem(0, nativeName);
        ui->comboBox->setCurrentIndex(0);
    }
}

QString SwitchWorkspaceDialog::currentWorkspace() const
{
    return QDir::cleanPath(QDir::fromNativeSeparators(ui->comboBox->currentText()));
}

void SwitchWorkspaceDialog::handleBrowseClicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this
                                                        , tr("Select directory for use as workspace")
                                                        , ui->comboBox->currentText()
                                                        );
    if (!dirName.isEmpty()) {
        setCurrentWorkspace(dirName);
    }
}

void SwitchWorkspaceDialog::handleAccepted()
{
    QStringList list;
    for (int i=0; i<ui->comboBox->count(); i++) {
        list << QDir::fromNativeSeparators(ui->comboBox->itemText(i));
    }
    settings_->setValue(WorkspacesListKey, list);
    settings_->setValue(CurrentWorkspaceKey, currentWorkspace());
    settings_->setValue(SkipChooseWorkspaceKey, ui->checkBox->isChecked());
}

SwitchWorkspaceDialog::~SwitchWorkspaceDialog()
{
    delete ui;
}

} // namespace CoreGUI
