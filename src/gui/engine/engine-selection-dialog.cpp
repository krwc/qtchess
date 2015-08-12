#include "engine-selection-dialog.hpp"
#include "settings/settings-factory.hpp"
#include <QVBoxLayout>

EngineSelectionDialog::EngineSelectionDialog(QWidget* parent)
    : QDialog(parent)
    , m_list(new EngineListWidget(this))
    , m_cancelButton(new QPushButton("Cancel"))
{
    setWindowTitle("Select engine");
    setMinimumSize(200, 200);
    setLayout(new QVBoxLayout());
    layout()->addWidget(m_list);
    layout()->addWidget(m_cancelButton);

    QObject::connect(m_list, &QListWidget::doubleClicked, this, &EngineSelectionDialog::onDoubleClicked);
    QObject::connect(m_cancelButton, &QPushButton::clicked, this, &EngineSelectionDialog::reject);
}

void EngineSelectionDialog::onDoubleClicked()
{
    m_engineName = m_list->engineName();
    emit accept();
}
