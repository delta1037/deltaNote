#ifndef GRAPHICSCOLORSVGITEM_H
#define GRAPHICSCOLORSVGITEM_H

#include <QGraphicsSvgItem>
#include <QDomDocument>
#include <QList>
#include <QPainter>
#include <QIcon>

class GraphicsColorSvgItem : public QGraphicsSvgItem
{
public:
    GraphicsColorSvgItem(QString filePath, QGraphicsItem *parent = nullptr);
    virtual ~GraphicsColorSvgItem();

    QIcon setColor(QColor c);

private:
    QDomDocument _svgXML;

    void changeAttributes(QString attName, QString attValue);
    void recursiveChangeAttributes(QDomNode node, QString attName, QString attValue);
};

#endif // GRAPHICSCOLORSVGITEM_H
