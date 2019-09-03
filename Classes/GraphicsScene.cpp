/*
 * GraphicsScene.cpp
 *
 *  Created on: May 4, 2015
 *      Author: Administrator
 */

#include "E:\cocos2d-x-2.2.6\samples\Cpp\HelloCpp\Classes\GraphicsScene.h"
#include "AppMacros.h"

USING_NS_CC;

int counter=0;
CCArray *noOfBrokenEggs;
CCLabelTTF* counterLabel;
//static function that will be called from AppDelegate.cpp to load your customized scene
CCScene* GraphicsScene::scene()
{

    CCScene *scene = CCScene::create();
    GraphicsScene *layer = GraphicsScene::create();
    scene->addChild(layer);
    return scene;
}


bool GraphicsScene::init()
{
	  CCSize Sizevisible = CCDirector::sharedDirector()->getVisibleSize();
	    CCPoint origin_ = CCDirector::sharedDirector()->getVisibleOrigin();

    ///....... Init function is called automatically when the object of your class is initiated
	//InitWithColor fucntion sets the background color of your scenes. Here we set it to White
    if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) )
    {
        return false;
    }

    CCSprite* bgSprite = CCSprite::create("bg.png");

       // position the sprite on the center of the screen
       bgSprite->setPosition(ccp(Sizevisible.width/2 + origin_.x, Sizevisible.height/2 + origin_.y));

       CCSize tTargetSize = CCSize( 100.0f, 200.0f );
       CCSize tSizeOrig = bgSprite->getContentSize();
      // bgSprite->setScale( ( tTargetSize.width / tSizeOrig.width ), ( tTargetSize.height / tSizeOrig.height ) );
       bgSprite->setScaleX(2);
         bgSprite->setScaleY(1.6);
       // add the sprite as a child to this layer
       this->addChild(bgSprite, 0);

    this->setColor(ccc3(255, 255, 255));
    enemies = new CCArray();
    bullets = new CCArray();
 noOfBrokenEggs= new CCArray();
    sprite  = CCSprite::create("Animate(21).png",  CCRectMake(0,0,64,64)); // NEW - create a sprite here
    sprite->setPosition(ccp(240,54)); //set position of sprite in some visible area
    this->addChild(sprite, 1); // cross check the Z index = 1 with your code

    //startAnimation(); // Function that is used for Animation without sprite sheet

    // NEW - create a sprite here
    CCSprite* basket = CCSprite::create("basket.png",CCRectMake(0,0,64,64));
    basket->setPosition(ccp(440,60)); //set position of sprite in some visible area
       this->addChild(basket, 1); // cross check the Z index = 1 with your code


    //Get the size of the visible screen
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();



    //Add a sprite that will be used for drag drop fucntionality
//    dragSprite = CCSprite::create("one.png");
//    dragSprite->setPosition(ccp(100,50));
//    this->addChild(dragSprite);


    //add player of the screen that will shoot bullets to kill enemies
//    CCSprite* pSprite = CCSprite::create("plyr.png");
//    pSprite->setPosition(ccp(visibleSize.width/4, visibleSize.height/2 + origin.y));
//    this->addChild(pSprite, 0);

    //schedule the function startEnemies that will continue generating enemy sprites after each second
    this->schedule( schedule_selector(GraphicsScene::startEnemies), 1.0 );

    //schedule the function update. The bullet and enemy collision will be checked continuously in this function
    this->schedule(schedule_selector(GraphicsScene::update));

    //set the screen touchable
    this->setTouchEnabled(true);
    return true;
}


//Function that generates animation with sprite sheet
void GraphicsScene::startAnimationWithSpriteSheet()
{

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	cache->addSpriteFramesWithFile("anim.plist");

	CCArray* animFrames = CCArray::createWithCapacity(10);

	char str[100] = {0};
	for (int i = 1; i <= 20; i++) {
		sprintf(str, "Animate(%d).png", i);
		CCSpriteFrame* frame = cache->spriteFrameByName( str );

		    animFrames->addObject(frame);
	 }
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames,0.1f);

	sprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));

	CCFiniteTimeAction* actionMove = CCMoveTo::create( 2,ccp(100, 100) );
	sprite->runAction(actionMove);

}

void GraphicsScene::startEnemies()
{
	this->addTarget();
}

void GraphicsScene::addTarget()
{

	//left egg
    CCSprite *target = CCSprite::create("egg.png",
        CCRectMake(0,0,20,20) );

    // Determine where to spawn the target along the Y axis
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int minY = target->getContentSize().height/2;
    int maxY = winSize.height
                          -  target->getContentSize().height/2;
    int rangeY = maxY - minY;
    // srand( TimGetTicks() );
    int actualY = ( rangeY ) + minY-125;

    // Create the target slightly off-screen along the right edge,
    // and along a random position along the Y axis as calculated
    target->setPosition(
        ccp(((target->getContentSize().width/2))+160,
        actualY-4) );
    this->addChild(target);
    target->setTag(1);
    enemies->addObject(target);

    // Determine speed of the target
    int minDuration = (int)2.0;
    int maxDuration = (int)4.0;
    int rangeDuration = maxDuration - minDuration;
    // srand( TimGetTicks() );
    int actualDuration = ( rand() % rangeDuration )
                                        + minDuration;

    // Create the actions
    CCFiniteTimeAction* actionMove =
        CCMoveTo::create( (float)actualDuration,
        ccp((rand()%300+(target->getContentSize().width/2)+50), 0) );

    CCFiniteTimeAction* actionMoveDone =
        CCCallFuncN::create( this,
        callfuncN_selector(GraphicsScene::spriteMoveFinished));

    target->runAction( CCSequence::create(actionMove,
        actionMoveDone, NULL) );



    //right egg
    CCSprite *target_r = CCSprite::create("egg.png",
           CCRectMake(0,0,20,20) );

       // Determine where to spawn the target along the Y axis
       CCSize winSize_r = CCDirector::sharedDirector()->getWinSize();
       int minY_r = target_r->getContentSize().height/2;
       int maxY_r = winSize.height
                             -  target_r->getContentSize().height/2;
       int rangeY_r = maxY_r - minY_r;
       // srand( TimGetTicks() );
       int actualY_r = ( rangeY_r ) + minY_r-125;

       // Create the target slightly off-screen along the right edge,
       // and along a random position along the Y axis as calculated
       target_r->setPosition(
           ccp(((target_r->getContentSize().width/2))+308,
           actualY_r-4) );
       this->addChild(target_r);
       target->setTag(1);
       enemies->addObject(target_r);

       // Determine speed of the target
       int minDuration_r = (int)2.0;
       int maxDuration_r = (int)4.0;
       int rangeDuration_r = maxDuration_r - minDuration_r;
       // srand( TimGetTicks() );
       int actualDuration_r = ( rand() % rangeDuration_r )
                                           + minDuration_r;

       // Create the actions
       CCFiniteTimeAction* actionMove_r =
           CCMoveTo::create( (float)actualDuration_r,
           ccp((rand()%300+(target_r->getContentSize().width/2)+50), 0) );

       CCFiniteTimeAction* actionMoveDone_r =
           CCCallFuncN::create( this,
           callfuncN_selector(GraphicsScene::spriteMoveFinished));

       target_r->runAction( CCSequence::create(actionMove_r,
           actionMoveDone_r, NULL) );





}

// This function is called when the enemies move ends and enemy has to be removed from the screen
void GraphicsScene::spriteMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	  this->removeChild(sprite, true);

	  if (sprite->getTag() == 1)  // target
	  {
		 if(sprite->getPosition().y==0)
		 {
			 CCSprite * brokenegg=CCSprite::create("egg_break.png");
			 brokenegg->setPosition(ccp(sprite->getPosition().x,30));
			noOfBrokenEggs->addObject(brokenegg);
			 this->addChild(brokenegg);
			 counter++;

			 if(counter==1)
			 {
				 this->removeChild(counterLabel);
				 CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
				 CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
				  counterLabel = CCLabelTTF::create("Egg broken: 1", "Arial", TITLE_FONT_SIZE);

				    // position the label on the center of the screen
				    counterLabel->setPosition(ccp(origin.x + visibleSize.width/2,
				                            origin.y + visibleSize.height - counterLabel->getContentSize().height));

				    // add the label as a child to this layer
				    this->addChild(counterLabel, 1);
			 }
			 if(counter==2)
						 {
				 this->removeChild(counterLabel);
							 CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
							 CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
							 counterLabel = CCLabelTTF::create("Egg broken: 2", "Arial", TITLE_FONT_SIZE);

							    // position the label on the center of the screen
							    counterLabel->setPosition(ccp(origin.x + visibleSize.width/2,
							                            origin.y + visibleSize.height - counterLabel->getContentSize().height));

							    // add the label as a child to this layer
							    this->addChild(counterLabel, 1);
						 }
			 if(counter==3)
						 {
				 this->removeChild(counterLabel);
							 CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
							 CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
							  counterLabel = CCLabelTTF::create("Egg broken: 3", "Arial", TITLE_FONT_SIZE);

							    // position the label on the center of the screen
							    counterLabel->setPosition(ccp(origin.x + visibleSize.width/2,
							                            origin.y + visibleSize.height - counterLabel->getContentSize().height));

							    // add the label as a child to this layer
							    this->addChild(counterLabel, 1);
						 }
			 if(counter==4)
						 {
				 this->removeChild(counterLabel);
							 CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
							 CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
							 counterLabel = CCLabelTTF::create("Egg broken: 4", "Arial", TITLE_FONT_SIZE);

							    // position the label on the center of the screen
							    counterLabel->setPosition(ccp(origin.x + visibleSize.width/2,
							                            origin.y + visibleSize.height - counterLabel->getContentSize().height));

							    // add the label as a child to this layer
							    this->addChild(counterLabel, 1);
						 }
			 if(counter==5)
						 {
				 this->removeChild(counterLabel);
							 CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
							 CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
							 counterLabel = CCLabelTTF::create("Egg broken: 5", "Arial", TITLE_FONT_SIZE);

							    // position the label on the center of the screen
							    counterLabel->setPosition(ccp(origin.x + visibleSize.width/2,
							                            origin.y + visibleSize.height - counterLabel->getContentSize().height));

							    // add the label as a child to this layer
							    this->addChild(counterLabel, 1);
						 }
			 if(counter==6)
						 {
				 this->removeChild(counterLabel);
							 CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
							 CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
							 counterLabel = CCLabelTTF::create("Egg broken: 6", "Arial", TITLE_FONT_SIZE);

							    // position the label on the center of the screen
							    counterLabel->setPosition(ccp(origin.x + visibleSize.width/2,
							                            origin.y + visibleSize.height - counterLabel->getContentSize().height));

							    // add the label as a child to this layer
							    this->addChild(counterLabel, 1);
						 }
			 if(counter==7)
						 {
				 this->removeChild(counterLabel);
							 CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
							 CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
							 counterLabel = CCLabelTTF::create("Egg broken: 7", "Arial", TITLE_FONT_SIZE);

							    // position the label on the center of the screen
							    counterLabel->setPosition(ccp(origin.x + visibleSize.width/2,
							                            origin.y + visibleSize.height - counterLabel->getContentSize().height));

							    // add the label as a child to this layer
							    this->addChild(counterLabel, 1);
						 }
			 if(counter==8)
						 {
				 this->removeChild(counterLabel);
							 CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
							 CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
							 counterLabel = CCLabelTTF::create("Egg broken: 8", "Arial", TITLE_FONT_SIZE);

							    // position the label on the center of the screen
							    counterLabel->setPosition(ccp(origin.x + visibleSize.width/2,
							                            origin.y + visibleSize.height - counterLabel->getContentSize().height));

							    // add the label as a child to this layer
							    this->addChild(counterLabel, 1);
						 }
			 if(counter==9)
						 {
				 this->removeChild(counterLabel);
							 CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
							 CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
							 counterLabel = CCLabelTTF::create("Egg broken: 9", "Arial", TITLE_FONT_SIZE);

							    // position the label on the center of the screen
							    counterLabel->setPosition(ccp(origin.x + visibleSize.width/2,
							                            origin.y + visibleSize.height - counterLabel->getContentSize().height));

							    // add the label as a child to this layer
							    this->addChild(counterLabel, 1);
						 }
			        if(counter==10)
			        {
			        	 CCSize Sizevisible = CCDirector::sharedDirector()->getVisibleSize();
			        		    CCPoint origin_ = CCDirector::sharedDirector()->getVisibleOrigin();
			        	 CCSprite* gmSprite = CCSprite::create("GameOver.jpg");

			        	       // position the sprite on the center of the screen
			        	       gmSprite->setPosition(ccp(Sizevisible.width/2 + origin_.x, Sizevisible.height/2 + origin_.y));

			        	 gmSprite->setScaleX(5);
			        	         gmSprite->setScaleY(5);
			        	 this->addChild(gmSprite, 1);
			        	//  sleep(8);
			        	//  exit(0);

			        }



	    enemies->removeObject(sprite);
		 }
	}
	  else if (sprite->getTag() == 2) // projectile
	  {
	    bullets->removeObject(sprite);
	  }
}

// This function is called when user drags on the screen
void GraphicsScene::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
		    CCPoint location = touch->getLocation();
		    CCArray* targetsToDelete =new CCArray();
		       CCObject* it = NULL;
		       CCObject* jt = NULL;
		    CCARRAY_FOREACH(noOfBrokenEggs, jt)
		           {
		               CCSprite *target = (CCSprite *)jt;

		               if (location.y<=40)
		               {
		            	   counter=0;
		                   targetsToDelete->addObject(target);
		                   this->removeChild(counterLabel);
		                  							 CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		                  							 CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
		                  							 counterLabel = CCLabelTTF::create("Egg broken: 0", "Arial", TITLE_FONT_SIZE);

		                  							    // position the label on the center of the screen
		                  							    counterLabel->setPosition(ccp(origin.x + visibleSize.width/2,
		                  							                            origin.y + visibleSize.height - counterLabel->getContentSize().height));

		                  							    // add the label as a child to this layer
		                  							    this->addChild(counterLabel, 1);
		        //           projectilesToDelete->addObject(projectile);
		               }
		           }


		       CCARRAY_FOREACH(targetsToDelete, jt)
		       {
		           CCSprite *target = (CCSprite *)jt;
		           noOfBrokenEggs->removeObject(target);
		           this->removeChild(target, true);

		       }

//		       CCARRAY_FOREACH(projectilesToDelete, it)
//		       {
//		           //CCSprite* projectile = (CCSprite *)it;
//		          // bullets->removeObject(projectile);
//		           //this->removeChild(projectile, true);
//		       }


		     //  projectilesToDelete->release();
		       targetsToDelete->release();




		    if(location.y>=0 && location.y<=100)
		    	{sprite->setPosition(location);}

}


void GraphicsScene::startAnimation()
{

	    cocos2d::CCAnimation* anim = CCAnimation::create();

	    // this is the most basic using one image per frame.

	    char str[100]={0};
	    for(int i = 0; i<40;i++)
	    {
	    	sprintf(str, "Animate(%d).png", i+1);
	    	 anim->addSpriteFrameWithFileName(str);
	    }

	    anim->setLoops(-1); // for infinit loop animation
	    anim->setDelayPerUnit(0.1f); //Duration per frame

	    sprite->runAction(CCAnimate::create(anim) );

	    CCFiniteTimeAction* actionMove = CCMoveTo::create( 5,ccp(100, 100) );
	    	sprite->runAction(actionMove);


}

//This function is called when touch ends

void GraphicsScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	//startAnimationWithSpriteSheet();

//		CCTouch* touch = (CCTouch*)( touches->anyObject() );
//	    CCPoint location = touch->getLocation();
//
//	    location = CCDirector::sharedDirector()->convertToGL(location);
//
//	    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
//
//	    // Set up initial location of projectile
//	    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
//
//
//	    CCSprite *projectile = CCSprite::create("egg.png",CCRectMake(0, 0, 20, 20));
//	    projectile->setPosition(ccp(100, winSize.height/2));
//
//	    // Determinie offset of location to projectile
//	    int offX = location.x - projectile->getPosition().x;
//	    int offY = location.y - projectile->getPosition().y;
//
//	    // Bail out if we are shooting down or backwards
//	    if (offX <= 0) return;
//
//	    // Ok to add now - we've double checked position
//	    this->addChild(projectile);
//
//	    projectile->setTag(2);
//	    bullets->addObject(projectile);
//
//	    // Determine where we wish to shoot the projectile to
//	    int realX = winSize.width + (projectile->getContentSize().width/2);
//	    float ratio = (float)offY / (float)offX;
//	    int realY = (realX * ratio) + projectile->getPosition().y;
//	    CCPoint realDest = ccp(realX, realY);
//
//	    // Determine the length of how far we're shooting
//	    int offRealX = realX - projectile->getPosition().x;
//	    int offRealY = realY - projectile->getPosition().y;
//	    float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
//	    float velocity = 480/1; // 480pixels/1sec
//	    float realMoveDuration = length/velocity;
//
//	    // Move projectile to actual endpoint
//	    projectile->runAction( CCSequence::create(CCMoveTo::create(realMoveDuration, realDest),
//	    		CCCallFuncN::create(this,callfuncN_selector(GraphicsScene::spriteMoveFinished)),
//	        NULL) );
}

//Check collision in this function. We scheduled this function in the init fucntion
void GraphicsScene::update(float dt)
{
    CCArray *projectilesToDelete = new CCArray();
    CCArray* targetsToDelete =new CCArray();
    CCObject* it = NULL;
    CCObject* jt = NULL;

    //CCARRAY_FOREACH(sprite, it)
    {
        CCSprite *projectile = (CCSprite *) sprite;
        CCRect projectileRect = CCRectMake(
                                           projectile->getPosition().x - (projectile->getContentSize().width/2),
                                           projectile->getPosition().y - (projectile->getContentSize().height/2),
                                           projectile->getContentSize().width,
                                           projectile->getContentSize().height);


        CCARRAY_FOREACH(enemies, jt)
        {
            CCSprite *target = (CCSprite *)jt;
            CCRect targetRect = CCRectMake(
                                           target->getPosition().x - (target->getContentSize().width/2),
                                           target->getPosition().y - (target->getContentSize().height/2),
                                           target->getContentSize().width,
                                           target->getContentSize().height);

            if (projectileRect.intersectsRect(targetRect))
            {
                targetsToDelete->addObject(target);
                projectilesToDelete->addObject(projectile);
            }
        }
    }

    CCARRAY_FOREACH(targetsToDelete, jt)
    {
        CCSprite *target = (CCSprite *)jt;
        enemies->removeObject(target);
        this->removeChild(target, true);

    }

    CCARRAY_FOREACH(projectilesToDelete, it)
    {
        //CCSprite* projectile = (CCSprite *)it;
       // bullets->removeObject(projectile);
        //this->removeChild(projectile, true);
    }


    projectilesToDelete->release();
    targetsToDelete->release();
}
