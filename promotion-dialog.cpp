#include "promotion-dialog.hpp"
#include "ui_promotion-dialog.h"

PromotionDialog::PromotionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PromotionDialog)
    , mSelectedPiece(PIECE_NONE)
{
    ui->setupUi(this);
}

PromotionDialog::~PromotionDialog()
{
    delete ui;
}

