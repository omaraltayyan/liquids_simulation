#include "Body.h"

Body::Body(const Body& p) : Body(p.boundingRect) {
}

Body::Body(QRectF boundingRect): boundingRect(boundingRect) {
}

Body::Body() {

}
