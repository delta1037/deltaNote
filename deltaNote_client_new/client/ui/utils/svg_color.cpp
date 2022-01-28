#include <QSvgRenderer>
#include <QRgb>
#include <QFile>
#include "svg_color.h"

SvgColor::SvgColor(QString filePath, QGraphicsItem *parent) :
    QGraphicsSvgItem(parent),
    _svgXML()
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QString baData = file.readAll();
    _svgXML.setContent(baData);
    setSharedRenderer(new QSvgRenderer(_svgXML.toByteArray()));
}

SvgColor::~SvgColor()
{
    delete renderer();
}

QIcon SvgColor::setColor(QColor c)
{
    change_attr("fill", c.name().toUpper());
    change_attr("stroke", c.name().toUpper());
    renderer()->load(_svgXML.toByteArray());

    // create pixmap target (could be a QImage)
    QPixmap pix(renderer()->defaultSize());
    pix.fill(Qt::transparent);
    // create painter to act over pixmap
    QPainter pixPainter(&pix);
    // use renderer to render over painter which paints on pixmap
    renderer()->render(&pixPainter);
    QIcon myicon(pix);
    // Use icon ....
    return myicon;
}

void SvgColor::change_attr(QString attName, QString attValue)
{
    QDomElement rootElem = _svgXML.documentElement();

    QDomNode n = rootElem.firstChild();
    while(!n.isNull())
    {
        if(n.isElement())
        {
            QDomElement e = n.toElement();
            if(e.hasAttribute(attName))
                e.setAttribute(attName, attValue);

            if(n.hasChildNodes())
                recur_change_attr(n.firstChild(), attName, attValue);
        }
        n = n.nextSibling();
    }
}

void SvgColor::recur_change_attr(QDomNode node, QString attName, QString attValue)
{
    QDomNode n = node;
    while(!n.isNull())
    {
        if(n.isElement())
        {
            QDomElement e = n.toElement();
            if(e.hasAttribute(attName))
                e.setAttribute(attName, attValue);

            if(n.hasChildNodes())
                recur_change_attr(n.firstChild(), attName, attValue);
        }
        n = n.nextSibling();
    }
}
