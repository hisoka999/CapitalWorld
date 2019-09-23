#include "engine/ui/ImageButton.h"
#include <engine/graphics/TextureManager.h>

namespace UI {

ImageButton::ImageButton(UI::Object *parent, int width, int height, int x2,
        int y2, bool pResized) :
        UI::Object(parent, width, height) {
    image = nullptr;
    this->x2 = x2;
    this->y2 = y2;
    resized = pResized;
}

ImageButton::~ImageButton() {

}

void ImageButton::loadImage( std::string pFilename) {

    image = graphics::TextureManager::Instance().loadTexture(pFilename);


}

void ImageButton::handleEvents(core::Input *pInput) {
    if (image == nullptr)
        return;

//	SDL_Event e = pInput->getEvent();
    ButtonListener *listener =
            static_cast<UI::ButtonListener*>(this->getListener());
    //if (!this->getListener())
    //    return;
//	int tx;
//	int ty;
    graphics::Rect rect = displayRect();
    rect.width = getWidth();
    rect.height = getHeight();

//	if (getParent() != NULL) {
//		graphics::Rect displayRect = getParent()->displayRect();
//		tx = displayRect.x + getX();
//		ty = displayRect.y + getY();
//	} else {
//		tx = getX();
//		ty = getY();
//	}
    utils::Vector2 pos = pInput->getMousePostion();
    if (rect.intersects(pos)
            && pInput->isMouseButtonPressed(SDL_MOUSEBUTTONDOWN)) {
        this->fireFuncionCall("buttonClick");
        this->fireFuncionCall("buttonClickPara",
                utils::Vector2(pos.getX() - rect.x, pos.getY() - rect.y));
        if (this->getListener())
            listener->buttonClick();
    }
//
//	if (e.button.x >= tx && e.button.x <= tx + width && e.button.y >= ty
//			&& e.button.y <= ty + height) {
//		if (e.type == SDL_MOUSEBUTTONDOWN
//				&& e.button.button == SDL_BUTTON_LEFT) {
//
//		}
//	}
}

void ImageButton::render(core::Renderer *pRender, graphics::Texture *pTexture) {
    if (image == NULL)
        return;
    graphics::Rect srcRect;
    graphics::Rect destRect;
    destRect.x = getX();
    destRect.y = getY();
    destRect.width = getWidth();
    destRect.height = getHeight();
    srcRect.x = x2;
    srcRect.y = y2;
    srcRect.width = getWidth();
    srcRect.height = getHeight();
    if (resized) {
        srcRect.width = image->getWidth();
        srcRect.height = image->getHeight();

    }

    if (this->getParent() != NULL) {
        graphics::Rect displayRect = getParent()->displayRect();
        destRect.x += displayRect.x;
        destRect.y += displayRect.y;
    }

    image->render(pRender, srcRect, destRect);
}

} // namespace UI
