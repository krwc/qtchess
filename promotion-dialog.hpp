#ifndef PROMOTIONDIALOG_HPP
#define PROMOTIONDIALOG_HPP
#include "pieces.hpp"
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

    Piece getSelectedPiece() const {
        return mSelectedPiece;
    }
private slots:
    void selectedQueen()  { mSelectedPiece = PIECE_QUEEN; emit done(0); }
    void selectedRook()   { mSelectedPiece = PIECE_ROOK;  emit done(0); }
    void selectedBishop() { mSelectedPiece = PIECE_BISHOP; emit done(0); }
    void selectedKnight() { mSelectedPiece = PIECE_KNIGHT; emit done(0); }
private:
    Piece mSelectedPiece;
    Ui::PromotionDialog *ui;
};

#endif // PROMOTIONDIALOG_HPP
