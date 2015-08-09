#ifndef HTMLMOVETREEBUILDER_HPP
#define HTMLMOVETREEBUILDER_HPP
#include <QString>

/*! \brief Very simple move tree builder. */
class HtmlMoveTreeBuilder {
public:
    /*! \brief Adds move number string */
    HtmlMoveTreeBuilder& addMoveNumber(const QString& number);
    /*! \brief Adds clickable move string */
    HtmlMoveTreeBuilder& addMove(const QString& move, size_t uid = 0,
                                 bool isCurrentMove = false);
    /*! \brief Puts \a variant inside brackets ( ) with proper styling */
    HtmlMoveTreeBuilder& addVariant(const QString& variant);
    /*! \brief Returns builded html string without style-sheet. */
    QString html() const;
    /*! \brief Returns builded html string with style-sheet. */
    QString htmlWithStyle() const;
    /*! \brief Returns true when builder string is empty */
    bool isEmpty() const;
private:
    QString m_html;
};

#endif // HTMLMOVETREEBUILDER_HPP
