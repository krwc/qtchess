#ifndef PROMOTIONDIALOG_HPP
#define PROMOTIONDIALOG_HPP
#include "game/pieces.hpp"
#include <QDialog>

namespace Ui {
class PromotionDialog;
}

class PromotionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotionDialog(QWidget *parent = 0);
    ~PromotionDialog();

    Piece::Type selectedPieceType() const {
        return mSelectedPieceType;
    }
private slots:
    void selectedQueen()  { mSelectedPieceType = Piece::Queen; emit done(0); }
    void selectedRook()   { mSelectedPieceType = Piece::Rook;  emit done(0); }
    void selectedBishop() { mSelectedPieceType = Piece::Bishop; emit done(0); }
    void selectedKnight() { mSelectedPieceType = Piece::Knight; emit done(0); }
private:
    Piece::Type mSelectedPieceType;
    Ui::PromotionDialog *ui;
};

#endif // PROMOTIONDIALOG_HPP
