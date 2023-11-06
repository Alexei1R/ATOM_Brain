#include "Ogre.h"
#include "OgreApplicationContext.h"

//! [key_handler]
class KeyHandler : public OgreBites::InputListener
{
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override
    {
        if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
        {
            Ogre::Root::getSingleton().queueEndRendering();
        }
        return true;
    }
};
//! [key_handler]

int main(int argc, char *argv[])
{
//! [constructor]
    OgreBites::ApplicationContext ctx("OgreTutorialApp");
    ctx.initApp();
//! [constructor]

//! [setup]
    // get a pointer to the already created root
    Ogre::Root* root = ctx.getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // Create a light
    Ogre::Light* light = scnMgr->createLight("MainLight");
    light->setCastShadows(true);
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // Create a camera and position it
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 2, 10);
    camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);

    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(0.1); // Adjust near clip distance
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);

    // Set up viewport
    ctx.getRenderWindow()->addViewport(cam);

    Ogre::ResourceGroupManager& resourceGroupManager = Ogre::ResourceGroupManager::getSingleton();
    resourceGroupManager.addResourceLocation("/home/toor/Desktop/ogre/Samples/Media/models", "FileSystem", "OgreInternal");



    // Load and display a 3D model
    Ogre::Entity* ent = scnMgr->createEntity("fish.mesh"); // Use a different model
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
//! [setup]

//! [main]
    // Register for input events
    KeyHandler keyHandler;
    ctx.addInputListener(&keyHandler);

    // Start rendering
    ctx.getRoot()->startRendering();
    ctx.closeApp();
//! [main]

    return 0;
}
