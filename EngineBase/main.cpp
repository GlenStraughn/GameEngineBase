
//-------------------------------------------------------------------------//
// Transforms - Tests using transforms for objects
// David Cline
// June 27, 2014
//-------------------------------------------------------------------------//

#include "EngineUtil.h"
#include "Scene.h"
#include "generateScript.h"
#include "Timer.hpp"

//-------------------------------------------------------------------------//
// Callback for Keyboard Input
//-------------------------------------------------------------------------//

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	printf("%c ", (char)key);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS &&
		((key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9'))) {
		printf("\n%c\n", (char)key);
	}
}

void loadPhysicsProperties(FILE* F, Scene* scene, SceneNode &sNode);

//-------------------------------------------------------------------------//
// Global State. 
//-------------------------------------------------------------------------//

GLFWwindow* gWindow = NULL;
string gWindowTitle = "Haywire Studios";
int gWidth = 600; // window width
int gHeight = 600; // window height
int gSPP = 16; // samples per pixel

//ISoundEngine* soundEngine = NULL;
//ISound* music = NULL;

Scene gScene;

bool g_switchCamera;

//-------------------------------------------------------------------------//
// Keyboard camera controller
//-------------------------------------------------------------------------//

void keyboardCameraController(Camera &camera)
{
    /*
	float t = 0.025f;
	float r = 0.01f;

    glm::vec3 translationVec;
    
	if (glfwGetKey(gWindow, 'Q'))
    {
        translationVec = {-t, 0, 0};
        camera.translateLocal(translationVec);
    }
	if (glfwGetKey(gWindow, 'E'))
    {
        translationVec = {t, 0, 0};
        camera.translateLocal(translationVec);
    }
	if (glfwGetKey(gWindow, 'W'))
    {
        translationVec = {0, 0, -t};
        camera.translateLocal(translationVec);
    }
	if (glfwGetKey(gWindow, 'S'))
    {
        translationVec = {0, 0, t};
        camera.translateLocal(translationVec);
    }
	if (glfwGetKey(gWindow, 'Z'))
    {
        translationVec = {0, -t, 0};
        camera.translateLocal(translationVec);
    }
	if (glfwGetKey(gWindow, 'C'))
    {
        translationVec = {0, t, 0};
        camera.translateLocal(translationVec);
    }
	if (glfwGetKey(gWindow, 'A'))
    {
        camera.rotateGlobal(glm::vec3(0,1,0), r);
    }
	if (glfwGetKey(gWindow, 'D'))
    {
        camera.rotateGlobal(glm::vec3(0,1,0), -r);
    }
    if (glfwGetKey(gWindow, GLFW_KEY_UP))
    {
        camera.rotateLocal(glm::vec3(1,0,0), r);
    }
	if (glfwGetKey(gWindow, GLFW_KEY_DOWN))
    {
        camera.rotateLocal(glm::vec3(1,0,0), -r);
    }
    if (glfwGetKey(gWindow, GLFW_KEY_SPACE))
    {
        if(g_switchCamera)
        {
            gScene.currentCamera = (gScene.currentCamera + 1) % gScene.cameras.size();
            g_switchCamera = false;
        }
    }
    else
    {
        g_switchCamera = true;
    }
	camera.refreshTransform((float)gWidth, (float)gHeight);
     */
}

//-------------------------------------------------------------------------//
// Parse Scene File
//-------------------------------------------------------------------------//

string ONE_TOKENS = "{}[]()<>+-*/,;";

void loadWorldSettings(FILE *F)
{
	string token, t;
	while (getToken(F, token, ONE_TOKENS)) {
		//cout << "  " << token << endl;
		if (token == "}") break;
		if (token == "windowTitle") getToken(F, gWindowTitle, ONE_TOKENS);
		else if (token == "width") getInts(F, &gWidth, 1);
		else if (token == "height") getInts(F, &gHeight, 1);
		else if (token == "spp") getInts(F, &gSPP, 1);
	}

	// Initialize the window with OpenGL context
	gWindow = createOpenGLWindow(gWidth, gHeight, gWindowTitle.c_str(), gSPP);
	glfwSetKeyCallback(gWindow, keyCallback);
}

void loadSceneSettings(FILE *F, Scene *scene)
{
	string token, t;
	while (getToken(F, token, ONE_TOKENS)) {
		if (token == "}") {
			break;
		}
		else if (token == "backgroundColor") {
			getFloats(F, &scene->backgroundColor[0], 3);
		}
		else if (token == "backgroundMusic") {
			string fileName, fullFileName;
			getToken(F, fileName, ONE_TOKENS);
			getFullFileName(fileName, fullFileName);
			//ISound* music = soundEngine->play2D(fullFileName.c_str(), true);
		}
	}
}

void loadMesh(FILE *F, Scene *scene)
{
	string token;
	while (getToken(F, token, ONE_TOKENS)) {
		if (token == "}") {
			break;
		}
		else if (token == "file") {
			string fileName = "";
			getToken(F, fileName, ONE_TOKENS);
			TriMesh *mesh = new TriMesh();
			mesh->readFromPly(fileName, false);
			mesh->sendToOpenGL();
			scene->addMesh(mesh);
		}
	}
}

void loadMeshInstance(FILE *F, Scene *scene, SceneNode &node)
{
	string token;
	GLuint vertexShader = NULL_HANDLE;
	GLuint fragmentShader = NULL_HANDLE;
	GLuint shaderProgram = NULL_HANDLE;
	TriMeshInstance *meshInstance = new TriMeshInstance();
	node.addTriMeshInstance(*meshInstance);

	while (getToken(F, token, ONE_TOKENS)) {
		if (token == "}") {
			break;
		}
		else if (token == "vertexShader") {
			string vsFileName;
			getToken(F, vsFileName, ONE_TOKENS);
			vertexShader = loadShader(vsFileName.c_str(), GL_VERTEX_SHADER);
		}
		else if (token == "fragmentShader") {
			string fsFileName;
			getToken(F, fsFileName, ONE_TOKENS);
			fragmentShader = loadShader(fsFileName.c_str(), GL_FRAGMENT_SHADER);
		}
		else if (token == "texture") {
			string texAttributeName;
			getToken(F, texAttributeName, ONE_TOKENS);
			string texFileName;
			getToken(F, texFileName, ONE_TOKENS);
			RGBAImage *image = scene->getTexture(texFileName);
			if (image == NULL) {
				image = new RGBAImage();
				image->loadPNG(texFileName);
				image->sendToOpenGL();
				scene->addTexture(image);
			}
			NameIdVal<RGBAImage*> texref(texAttributeName, -1, image);
			meshInstance->mat.textures.push_back(texref);
		}
		else if (token == "mesh") {
			string meshName;
			getToken(F, meshName, ONE_TOKENS);
			TriMesh *mesh = scene->getMesh(meshName);
			if (mesh == NULL) {
				mesh = new TriMesh();
				mesh->readFromPly(meshName);
				mesh->sendToOpenGL();
				scene->addMesh(mesh);
			}
			meshInstance->setMesh(mesh);
		}
		else if (token == "translate") {
			glm::vec3 t;
			getFloats(F, &t[0], 3);
			meshInstance->T.translation += t;
		}
		else if (token == "scale") {
			glm::vec3 s;
			getFloats(F, &s[0], 3);
			meshInstance->T.scale *= s;
		}
        else if (token == "type")
        {
            string typeTag;
            getToken(F, typeTag, ONE_TOKENS);
            
            if(typeTag == "mesh")
            {
                meshInstance->meshType = TriMeshInstance::MeshDrawType::MESH;
            }
            else if(typeTag == "billboard")
            {
                meshInstance->meshType = TriMeshInstance::MeshDrawType::BILLBOARD;
            }
            else if(typeTag == "sprite" || typeTag == "pointSprite")
            {
                meshInstance->meshType = TriMeshInstance::MeshDrawType::POINT_SPRITE;
            }
        }
	}

	shaderProgram = createShaderProgram(vertexShader, fragmentShader);
	meshInstance->mat.shaderProgram = shaderProgram;
}

void loadCamera(FILE *F, Scene *scene)
{
	string token;
	Camera *camera = new Camera();

	while (getToken(F, token, ONE_TOKENS)) {
		if (token == "}") break;
		else if (token == "eye") getFloats(F, &(camera->eye[0]), 3);
		else if (token == "center") getFloats(F, &(camera->center[0]), 3);
		else if (token == "vup") getFloats(F, &(camera->vup[0]), 3);
		else if (token == "znear") getFloats(F, &(camera->znear), 1);
		else if (token == "zfar") getFloats(F, &(camera->zfar), 1);
		else if (token == "fovy") getFloats(F, &(camera->fovy), 1);
	}

	camera->refreshTransform((float)gWidth, (float)gHeight);
    scene->cameras.push_back(camera);
}

void loadLight(FILE *F, Scene *scene)
{
	string token;
	Light light;

	// position direction color attenuation coneAngles type

	while (getToken(F, token, ONE_TOKENS)) {
		if (token == "}") break;
		else if (token == "position") getFloats(F, &(light.position[0]), 3);
		else if (token == "color") getFloats(F, &(light.color[0]), 3);
		else if (token == "attenuation") getFloats(F, &(light.attenuation[0]), 3);
		else if (token == "coneAngles") {
			getFloats(F, &(light.coneAngles[0]), 2);
			light.coneAngles[0] = cos(light.coneAngles[0]);
			light.coneAngles[1] = cos(light.coneAngles[1]);
		}
		else if (token == "direction") {
			getFloats(F, &(light.direction[0]), 3);
			light.direction = glm::normalize(light.direction);
		}
		else if (token == "type") {
			string t;
			getToken(F, t, ONE_TOKENS);
			if (t == "point") light.attenuation.w = POINT_LIGHT;
			else if (t == "directional") light.attenuation.w = DIRECTIONAL_LIGHT;
			else if (t == "spot") light.attenuation.w = SPOT_LIGHT;
            else if (t == "ambient") light.attenuation.w = AMBIENT_LIGHT;
		}
	}

	// Add light to global light list
	gLights[gNumLights] = light;
	gNumLights++;
	if (gNumLights >= MAX_LIGHTS)
	{
		ERROR_MSG("Too many lights!", false);
		gNumLights--;
	}
}


void loadScript(FILE* F, Scene* scene, SceneNode &node)
{
    Script* newScript = NULL;
    string token;
    while (getToken(F, token, ONE_TOKENS))
    {
		if (token == "}")
        {
            break;
        }
        else if(token == "name" || token == "type" || token == "scriptType")
        {
            string scriptName;
            getToken(F, scriptName, ONE_TOKENS);
            
            newScript = generateScript(scriptName);
            node.addScript(*newScript);
        }
        else if(token == "float")
        {
            string variableName;
            float newFloat;
            
            getToken(F, variableName, ONE_TOKENS);
            getFloats(F, &newFloat, 1);
            
            if(newScript != NULL)
            {
                newScript->setFloatValue(variableName, newFloat);
            }
        }
        else if(token == "floatArray")
        {
            string variableName;
            float* newArray;
            float size;
            
            getToken(F, variableName, ONE_TOKENS);
            getFloats(F, &size, 1);
            
            newArray = new float[int(size)];
            getFloats(F, newArray, int(size));
            
            if(newScript != NULL)
            {
                newScript->setFloatArray(variableName, newArray);
            }
        }
        else if(token == "string")
        {
            string variableName, newString;
            
            getToken(F, variableName, ONE_TOKENS);
            getToken(F, newString, ONE_TOKENS);
            
            if(newScript != NULL)
            {
                newScript->setStringValue(variableName, newString);
            }
        }
        else if(token == "stringArray")
        {
            string variableName, *newArray;
            float size;
            
            getToken(F, variableName, ONE_TOKENS);
            getFloats(F, &size, 1);
            
            newArray = new string[int(size)];
            
            for(int i = 0; i < size; i++)
            {
                getToken(F, newArray[i], ONE_TOKENS);
            }
            
            
            if(newScript != NULL)
            {
                newScript->setStringArray(variableName, newArray);
            }
        }
        
    }
    
    loadSpecialCase(F, scene, *newScript);
}


void loadSceneNode(FILE* F, Scene* scene, SceneNode &node)
{
    cout << "loading scene node" << endl;
    string token;
    bool nodeAdded = false;
    
    SceneNode* newNode = new SceneNode();
    
    newNode->setParent(node);
    
    while (getToken(F, token, ONE_TOKENS)) {
		//cout << token << endl;
        if (token == "}") break;
        else if (token == "name" || token == "id" || token == "ID")
        {
            string name;
            getToken(F, name, ONE_TOKENS);
            
            newNode->setName(name);
            
            nodeAdded = scene->addNode(name, *newNode);
        }
        else if(nodeAdded)
        {
            if (token == "meshInstance") {
                loadMeshInstance(F, scene, *newNode);
            }
            else if (token == "node" || token == "sceneNode")
            {
                loadSceneNode(F, scene, *newNode);  // Sub-node initialization
            }
            else if (token == "translate") {
                glm::vec3 t;
                getFloats(F, &t[0], 3);
                newNode->setTranslation(t);
            }
            else if (token == "scale") {
                glm::vec3 s;
                getFloats(F, &s[0], 3);
                newNode->setScale(s);
            }
            else if(token == "script")
            {
                loadScript(F, scene, *newNode);
            }
            else if(token == "physics" || token == "physicsProperties")
            {
                loadPhysicsProperties(F, scene, *newNode);
            }
        }
    }
    
    node.addChild(newNode);
    
    cout << "scene node loaded" << endl;
}


void loadScene(const char *sceneFile, Scene *scene)
{
	string sceneFileName = sceneFile;
	int separatorIndex = sceneFileName.find_last_of("/");
	if (separatorIndex < 0) separatorIndex = sceneFileName.find_last_of("\\");
	if (separatorIndex > 0) addToPath(sceneFileName.substr(0, separatorIndex+1));
	//printf("PATH:\n");
	//printPath();

	FILE *F = openFileForReading(sceneFile);
	string token;

	while (getToken(F, token, ONE_TOKENS)) {
		//cout << token << endl;
		if (token == "worldSettings") {
			loadWorldSettings(F);
			initLightBuffer();
		}
		else if (token == "sceneSettings") {
			loadSceneSettings(F, scene);
		}
		else if (token == "mesh") {
			loadMesh(F, scene);
		}
		else if (token == "meshInstance") {
			loadMeshInstance(F, scene, scene->root);
		}
		else if (token == "camera") {
			loadCamera(F, scene);
		}
		else if (token == "light") {
			loadLight(F, scene);
		}
        else if (token == "node" || token == "sceneNode")
        {
            loadSceneNode(F, scene, scene->root);
        }
	}
}

//-------------------------------------------------------------------------//
// Update
//-------------------------------------------------------------------------//

void update(void)
{	
	/*
	// move mesh instance
	gMeshInstance.translation[0] += 0.003f;
	if (gMeshInstance.translation[0] >= 1.0f) gMeshInstance.translation[0] = -1.0f;
    */
	
	// scale mesh instance
	/*
	static float dScale = 0.0005f;
	float scale = gMeshInstance.T.scale[0];
	scale += dScale;
	if (scale > 1.25f) dScale = -0.0005f;
	if (scale < 0.25f) dScale = 0.0005f;
	gMeshInstance.setScale(glm::vec3(scale));

	// rotate mesh
	glm::quat r = glm::quat(glm::vec3(0.0f, 0.0051f, 0.00f));
	gMeshInstance.T.rotation *= r;
	*/
	
	/*
	gMeshInstance.diffuseColor += glm::vec4(0.0013f, 0.000921f, 0.00119f, 0.0f);
	if (gMeshInstance.diffuseColor[0] > 1.0f) gMeshInstance.diffuseColor[0] = 0.25f;
	if (gMeshInstance.diffuseColor[1] > 1.0f) gMeshInstance.diffuseColor[1] = 0.25f;
	if (gMeshInstance.diffuseColor[2] > 1.0f) gMeshInstance.diffuseColor[2] = 0.25f;
	*/
    gScene.runNodeScripts();
}

//-------------------------------------------------------------------------//
// Draw a frame
//-------------------------------------------------------------------------//

void render(void)
{
	gScene.render();
}

//-------------------------------------------------------------------------//
// Main method
//-------------------------------------------------------------------------//

int main(int numArgs, char **args)
{
	// check usage
	if (numArgs < 2) {
		cout << "Usage: sceneFile.scene" << endl;
		exit(0);
	}

	// Start sound engine
	/*soundEngine = irrklang::createIrrKlangDevice();
	if (!soundEngine) return 0;
	soundEngine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 1));
	soundEngine->setSoundVolume(0.25f); // master volume control*/

	// Play 3D sound
	//string soundFileName;
	//ISound* music = soundEngine->play3D(soundFileName.c_str(), vec3df(0, 0, 10), true); // position and looping
	//if (music) music->setMinDistance(5.0f); // distance of full volume

	loadScene(args[1], &gScene);

	// start time (used to time framerate)
	double startTime = TIME();
    
    Timer logicTimer;
    logicTimer.setInterval(double(1.0)/double(100.0));
    Timer renderTimer;
    renderTimer.setInterval(double(1.0)/double(60.0));
    
    logicTimer.resetCycle();
    renderTimer.resetCycle();
    
	// render loop
	while (true) {
		// update and render
        
        if(logicTimer.hasCyclePassed())
        {
            update();
            logicTimer.resetCycle();
        }
        
        if(renderTimer.hasCyclePassed())
        {
            render();
            glfwGetWindowSize(gWindow, &gWidth, &gHeight);
        
            // handle input
            glfwPollEvents();
            if (glfwWindowShouldClose(gWindow) != 0) break;
            keyboardCameraController(*gScene.cameras[gScene.currentCamera]);

            double xx, yy;
            glfwGetCursorPos(gWindow, &xx, &yy);
            printf("%1.3f %1.3f ", xx, yy);
        
            // print framerate
            double endTime = TIME();
            printf("\rFPS: %1.0f  ", 1.0/(endTime-startTime));
            startTime = endTime;
        
            // swap buffers
            //SLEEP(10); // sleep 1 millisecond to avoid busy waiting
            glfwSwapBuffers(gWindow);
            
            Camera* c = gScene.cameras[gScene.currentCamera];
            
            printVec(c->eye);
            cout << endl;
            
            renderTimer.resetCycle();
        }
	}

	// Shut down sound engine
	//if (music) music->drop(); // release music stream.
	//soundEngine->drop(); // delete engine
    
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

//-------------------------------------------------------------------------//



void loadPhysicsProperties(FILE* F, Scene* scene, SceneNode &sNode)
{
    string token;
    while (getToken(F, token, ONE_TOKENS))
    {
        if (token == "}")
        {
            break;
        }
        else if(token == "gravity" || token == "affectedByGravity")
        {
            string answer;
            
            getToken(F, answer, ONE_TOKENS);
            
            if(answer == "true" || answer == "yes")
            {
                sNode.toggleGravity(true);
            }
            else
            {
                sNode.toggleGravity(false);
            }
        }
        else if(token == "velocity")
        {
            glm::vec3 velocity;
            
            getFloats(F, &velocity[0], 3);
            
            sNode.setSpeed(velocity);
        }
        else if(token == "solid")
        {
            string answer;
            
            getToken(F, answer, ONE_TOKENS);
            
            if(answer == "true" || answer == "yes")
            {
                sNode.toggleSolid(true);
            }
            else
            {
                sNode.toggleSolid(false);
            }
        }
        else if(token == "restitution")
        {
            float res;
            getFloats(F, &res, 1);
            
            sNode.setRestitution(res);
        }
        else if(token == "body")
        {
            float array[3];
            
            getFloats(F, array, 3);
            sNode.setBody(array);
        }
        else if(token == "mobile")
        {
            string answer;
            
            getToken(F, answer, ONE_TOKENS);
            
            if(answer == "true" || answer == "yes")
            {
                sNode.setMobility(true);
            }
            else
            {
                sNode.setMobility(false);
            }
        }
    }
}