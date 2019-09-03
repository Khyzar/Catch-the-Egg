/*
 * GraphicsScene.h
 *
 *  Created on: May 4, 2015
 *      Author: Administrator
 */



#ifndef GRAPHICSSCENE_H_
#define GRAPHICSSCENE_H_

#include "cocos2d.h"


class GraphicsScene : public cocos2d::CCLayerColor
{
	cocos2d::CCArray *enemies;
	cocos2d::CCArray *bullets;
	cocos2d::CCSprite* dragSprite;
	cocos2d::CCSprite * sprite;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void startAnimation();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void startEnemies();
    void addTarget();
    void spriteMoveFinished(CCNode* sender);
    void update(float dt);
    void startAnimationWithSpriteSheet();

    // implement the "static node()" method manually
    CREATE_FUNC(GraphicsScene);
};


#endif /* GRAPHICSSCENE_H_ */
