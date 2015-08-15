#ifndef GAME_TREE_WIDGET_HPP
#define GAME_TREE_WIDGET_HPP
#include "game/tree.hpp"
#include <QWidget>
#include <QWebView>

class HtmlMoveTreeBuilder;
class TreeHtml
{
public:
    /*! \brief Returns html representation of the tree */
    static QString html(const Tree*);
private:
    static void traverse(HtmlMoveTreeBuilder& builder, Move lastMove, const TreeNode* node,
                         const Tree* tree);
};

class MoveTreeWidget : public QWebView {
    Q_OBJECT
public:
    explicit MoveTreeWidget(QWidget* parent = nullptr);

    /*! \brief Sets tree that will be rendered by this widget */
    void setTree(Tree* tree);

    /*! \brief Returns satisfactory size */
    virtual QSize sizeHint() const;
protected:
    /*! \brief Displays custom context menu */
    virtual void contextMenuEvent(QContextMenuEvent*);
public slots:
    /*! \brief Issues redraw */
    void redraw();
private slots:
    void onMoveClicked(const QUrl&);
    void onMoveHovered(const QString&);
    void onAnnotate();
    void onRemoveAnnotation();
    void onPromoteUp();
    void onPromoteToMainline();
    void onRemove();
signals:
    void moveSelected(size_t);
private:
    Tree* m_tree;
    /*!< Currently hovered node uid or 0. */
    size_t m_hoveredMoveUid;
    /*!< Uid used for action */
    size_t m_actionMoveUid;
};

#endif // GAME_TREE_WIDGET_HPP
