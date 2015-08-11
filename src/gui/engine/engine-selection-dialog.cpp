#include "engine-selection-dialog.hpp"
#include "settings/settings-factory.hpp"
#include <QVBoxLayout>

EngineSelectionDialog::EngineSelectionDialog(QWidget* parent)
    : QDialog(parent)
    , m_listWidget(new QListWidget(this))
    , m_cancelButton(new QPushButton("Cancel"))
{
    setWindowTitle("Select engine");
    setMinimumSize(200, 200);
    setLayout(new QVBoxLayout());
    layout()->addWidget(m_listWidget);
    layout()->addWidget(m_cancelButton);

    m_listWidget->addItems(EngineSettings::engines());

    QObject::connect(m_listWidget, &QListWidget::itemDoubleClicked, this, &EngineSelectionDialog::onDoubleClicked);
    QObject::connect(m_cancelButton, &QPushButton::clicked, this, &EngineSelectionDialog::reject);
}

void EngineSelectionDialog::onDoubleClicked(QListWidgetItem* item)
{
    m_selectedEngine = item->text();
    emit accept();
}
