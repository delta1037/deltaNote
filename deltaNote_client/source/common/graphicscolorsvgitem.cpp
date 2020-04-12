#include "common/graphicscolorsvgitem.h"

#include <QSvgRenderer>
#include <QRgb>
#include <QDebug>
#include <QFile>

GraphicsColorSvgItem::GraphicsColorSvgItem(QString filePath, QGraphicsItem *parent) :
    QGraphicsSvgItem(parent),
    _svgXML()
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QString baData = file.readAll();
    _svgXML.setContent(baData);
    setSharedRenderer(new QSvgRenderer(_svgXML.toByteArray()));
}

GraphicsColorSvgItem::~GraphicsColorSvgItem()
{
    delete renderer();
}

QIcon GraphicsColorSvgItem::setColor(QColor c)
{
    changeAttributes("fill", c.name().toUpper());
    changeAttributes("stroke", c.name().toUpper());
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

void GraphicsColorSvgItem::changeAttributes(QString attName, QString attValue)
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
                recursiveChangeAttributes(n.firstChild(), attName, attValue);
        }
        n = n.nextSibling();
    }
}

void GraphicsColorSvgItem::recursiveChangeAttributes(QDomNode node, QString attName, QString attValue)
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
                recursiveChangeAttributes(n.firstChild(), attName, attValue);
        }
        n = n.nextSibling();
    }
}
