
//-------------------------------------------------------------------------//
// MyUtil.h
// Some utility stuff - file reading and such
//
// David Cline
// 6/14/2014
//-------------------------------------------------------------------------//

// Local includes
#include "EngineUtil.h"
#include <math.h>

//-------------------------------------------------------------------------//
// MISCELLANEOUS
//-------------------------------------------------------------------------//

void ERROR_MSG(const string &msg, bool doExit)
{
	cerr << "\nERROR! " << msg << endl;
	if (doExit) exit(0);
}

double TIME(void)
{
	return (double)clock() / (double)CLOCKS_PER_SEC;
}

void SLEEP(int millis)
{
	this_thread::sleep_for(chrono::milliseconds(millis));
}

//-------------------------------------------------------------------------//
// OPENGL STUFF
//-------------------------------------------------------------------------//

GLFWwindow* createOpenGLWindow(int width, int height, const char *title, int samplesPerPixel)
{
	// Initialise GLFW
	if (!glfwInit()) ERROR_MSG("Failed to initialize GLFW.", true);
	glfwWindowHint(GLFW_SAMPLES, samplesPerPixel);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
	// Open a window and create its OpenGL context
	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		ERROR_MSG("Failed to open GLFW window.", true);
	}
	glfwMakeContextCurrent(window);
    
	// Ensure we can capture the escape key being pressed
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		ERROR_MSG("Failed to initialize GLEW.", true);
	}
    
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
    
	// Print the OpenGL version we are working with
	char *GL_version = (char *)glGetString(GL_VERSION);
	printf("OpenGL Version: %s\n", GL_version);
	return window;
}

//-------------------------------------------------------------------------//

GLuint loadShader(const string &fileName, GLuint shaderType)
{
	// load the shader as a file
	string mainCode;
	if (!loadFileAsString(fileName, mainCode)) {
		ERROR_MSG("Could not load file '" + fileName + "'", false);
		return NULL_HANDLE;
	}

	string shaderCode;
	string alreadyIncluded = fileName;
	replaceIncludes(mainCode, shaderCode, "#include", alreadyIncluded, true);

    
	// print the shader code
	#ifdef _DEBUG
	//cout << "\n----------------------------------------------- SHADER CODE:\n";
	//cout << shaderCode << endl;
	//cout << "--------------------------------------------------------------\n";
	#endif
    
	// transfer shader code to card and compile
	GLuint shaderHandle = glCreateShader(shaderType); // create handle for the shader
	const char* source = shaderCode.c_str();          // get C style string for shader code
	glShaderSource(shaderHandle, 1, &source, NULL);   // pass the shader code to the card
	glCompileShader(shaderHandle);                    // attempt to compile the shader
	
	// check to see if compilation worked
	// If the compilation did not work, print an error message and return NULL handle
	int status;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		ERROR_MSG("compiling shader '" + fileName + "'", false);				
		GLint msgLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &msgLength);
		std::vector<char> msg(msgLength);
		glGetShaderInfoLog(shaderHandle, msgLength, &msgLength, &msg[0]);
		printf("%s\n", &msg[0]);
		glDeleteShader(shaderHandle);
        exit(0);
		return NULL_HANDLE;
	}
    
	return shaderHandle;
}

//-------------------------------------------------------------------------//

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// Create and link the shader program
	GLuint shaderProgram = glCreateProgram(); // create handle
	if (!shaderProgram) {
		ERROR_MSG("could not create the shader program", false);
		return NULL_HANDLE;
	}
	glAttachShader(shaderProgram, vertexShader);    // attach vertex shader
	glAttachShader(shaderProgram, fragmentShader);  // attach fragment shader
	glLinkProgram(shaderProgram);
    
	// check to see if the linking was successful
	int linked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked); // get link status
	if (!linked) {
		ERROR_MSG("could not link the shader program", false);
		GLint msgLength = 0;
		glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &msgLength);
		std::vector<char> msg(msgLength);
		glGetShaderInfoLog(shaderProgram, msgLength, &msgLength, &msg[0]);
		printf("%s\n", &msg[0]);
		glDeleteProgram(shaderProgram);
		return NULL_HANDLE;
	}

	// HOOK UP UNIFORM BUFFER AND UNIFORM BLOCK TO SAME BINDING POINT
	GLint lightBlockIndex = glGetUniformBlockIndex(shaderProgram, "Lights");
	glUniformBlockBinding(shaderProgram, lightBlockIndex, LIGHT_BUFFER_ID);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHT_BUFFER_ID, gLightBufferObject);

	printf("LIGHT STUFF %d %d %d\n", shaderProgram, lightBlockIndex, gLightBufferObject);
    
	return shaderProgram;
}

//-------------------------------------------------------------------------//
// GLM UTILITY STUFF
//-------------------------------------------------------------------------//

void printMat(const glm::mat4x4 &m)
{
	printf("\n");
	for (int r = 0; r < 4; r++) {
		printf("[ ");
		for (int c = 0; c < 4; c++) {
			printf("%7.3f ", m[c][r]); // glm uses column major order
		}
		printf("]\n");
	}
}

//-------------------------------------------------------------------------//
// FILE READING
//-------------------------------------------------------------------------//

vector<string> PATH;

void addToPath(const string &p)
{
	PATH.push_back(p);
}

void removeFromPath(const string &p)
{
	for (int i = (int)p.length() - 1; i >= 0; i--) {
		if (PATH[i] == p) PATH.erase(PATH.begin() + i);
	}
}

void printPath(void)
{
	for (int i = 0; i < (int)PATH.size(); i++) {
		cout << PATH[i] << endl;
	}
}

bool getFullFileName(const string &fileName, string &fullName)
{
	for (int i = -1; i < (int)PATH.size(); i++) {
		if (i < 0) fullName = fileName;
		else fullName = PATH[i] + fileName;

		FILE *f = fopen(fullName.c_str(), "rb");
		if (f != NULL) {
			fclose(f);
			return true;
		}
	}
	fullName = "";
	return false;
}

FILE *openFileForReading(const string &fileName)
{
	string fullName;
	bool fileExists = getFullFileName(fileName, fullName);
	if (fileExists) {
		cout << "Opening file '" << fileName << "'" << endl;
		return fopen(fullName.c_str(), "rb");
	}

	string msg = "Could not open file " + fileName;
	ERROR_MSG(msg.c_str(), false);
	return NULL;
}

//-------------------------------------------------------------------------//

bool getToken(FILE *f, string &token, const string &oneCharTokens)
{
	token = "";
	while (!feof(f)) {
		int c = getc(f);
		int tokenLength = (int)token.length();
		int cIsSpace = isspace(c);
		bool cIsOneCharToken = ((int)oneCharTokens.find((char)c) >= 0);
        
		if (cIsSpace && tokenLength == 0) { // spaces before token, ignore
			continue;
		}
		else if (cIsSpace) { // space after token, done
			break;
		}
		else if (c == EOF) { // end of file, done
			break;
		}
		else if ((tokenLength == 0) && cIsOneCharToken) { // oneCharToken, done
			token += (char)c;
			break;
		}
		else if (cIsOneCharToken) { // oneCharToken after another token, push back
			ungetc(c, f);
			break;
		}
		else if ((tokenLength == 0) && (c == '\"' || c == '\'')) { // quoted string, append til end quote found
			char endQuote = c;
			while (!feof(f)) {
				int d = getc(f);
				if (d == endQuote) return true;
				token += (char)d;
			}
			break;
		}
		else if (c == '\"' || c == '\'') { // quote after token started, push back
			ungetc(c, f);
			break;
		}
		else {
			token += (char)c;
		}
	}
	//cout << token << endl;
	return (token.length() > 0);
}

//-------------------------------------------------------------------------//

int getFloats(FILE *f, float *a, int num)
{
	string token;
	int count = 0;
	while (getToken(f, token, "[],")) {
		if (token == "]") {
			break;
		}
		else if (isdigit(token[0]) || token[0] == '-') {
			sscanf(token.c_str(), "%f", &a[count]);
			count++;
			if (count == num) break;
		}
	}
	return count;
}

int getInts(FILE *f, int *a, int num)
{
	string token;
	int count = 0;
	while (getToken(f, token, "[],")) {
		if (token == "]") {
			break;
		}
		else if (isdigit(token[0]) || token[0] == '-') {
			sscanf(token.c_str(), "%d", &a[count]);
			count++;
			if (count == num) break;
		}
	}
	return count;
}
//-------------------------------------------------------------------------//

bool loadFileAsString(const string &fileName, string &fileContents)
{
	printf("loading file '%s'\n", fileName.c_str());

	string fullName;
	bool fileExists = getFullFileName(fileName, fullName);
	if (!fileExists) return false;

	ifstream fileStream(fullName.c_str());
	if (fileStream.good()) {
		stringstream stringStream;
		stringStream << fileStream.rdbuf();
		fileContents = stringStream.str();
		fileStream.close();
		return true;
	}
	
	fileStream.close();
	return false;
}

//-------------------------------------------------------------------------//

void replaceIncludes(string &src, string &dest, const string &directive, 
	string &alreadyIncluded, bool onlyOnce)
{
	int start = 0;

	while (true) {
		int includeIndex = (int)src.find("#include", start);
		if (includeIndex < 0) {
			dest += src.substr(start);
			break;
		}
		if (includeIndex > 0 && !isspace(src[includeIndex - 1])) continue;
		dest += src.substr(start, includeIndex - 1);
		//
		int quoteStart = (int)src.find("\"", start + 8);
		int quoteEnd = (int)src.find("\"", quoteStart + 1);
		start = quoteEnd + 1;
		if (quoteStart >= quoteEnd) {
			ERROR_MSG("could not replace includes");
			break;
		}
		string includeFileName = src.substr(quoteStart + 1, (quoteEnd - quoteStart - 1));
		if ((int)alreadyIncluded.find(includeFileName) < 0) {
			if (onlyOnce) {
				alreadyIncluded.append("|");
				alreadyIncluded.append(includeFileName);
			}
			string subSource;
			loadFileAsString(includeFileName, subSource);
			replaceIncludes(subSource, dest, directive, alreadyIncluded, onlyOnce);
		}
	}
}

//-------------------------------------------------------------------------//
// RGBAImage
//-------------------------------------------------------------------------//

RGBAImage::~RGBAImage()
{
	if (textureId != NULL_HANDLE) glDeleteTextures(1, &textureId);
	if (samplerId != NULL_HANDLE) glDeleteSamplers(1, &samplerId);
}

bool RGBAImage::loadPNG(const string &fileName, bool doFlipY)
{
	string fullName;
	getFullFileName(fileName, fullName);
	unsigned error = lodepng::decode(pixels, width, height, fullName.c_str());
	if (error) {
		ERROR_MSG(lodepng_error_text(error), false);
		return false;
	}

	if (doFlipY) flipY(); // PNGs go top-to-bottom, OpenGL is bottom-to-top
	name = fileName;
	return true;
}

bool RGBAImage::writeToPNG(const string &fileName)
{
	unsigned error = lodepng::encode(fileName.c_str(), pixels, width, height);
	if (error) {
		ERROR_MSG(lodepng_error_text(error), false);
		return false;
	}
	return true;
}

void RGBAImage::flipY(void)
{
	unsigned int *a, *b;
	unsigned int temp;

	for (int y = 0; y < (int)height / 2; y++)
	{
		a = &pixel(0, y);
		b = &pixel(0, height - 1 - y);
		for (int x = 0; x < (int)width; x++) {
			temp = a[x];
			a[x] = b[x];
			b[x] = temp;
		}
	}
}

void RGBAImage::sendToOpenGL(GLuint magFilter, GLuint minFilter, bool createMipMap)
{
	if (width <= 0 || height <= 0) return;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
	if (createMipMap) glGenerateMipmap(GL_TEXTURE_2D);

	glGenSamplers(1, &samplerId);
	glBindSampler(textureId, samplerId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
}

//-------------------------------------------------------------------------//
// TRIANGLE MESH
//-------------------------------------------------------------------------//

bool TriMesh::readFromPly(const string &fileName, bool flipZ)
{
	name = fileName;
	FILE *f = openFileForReading(fileName);
	if (f == NULL) return false;
	string token, t;
	int numVertices = 0;
	int numFaces = 0;
	int numTriangles = 0;
	vector<int> faceIndices;
    
	// get num vertices
	while (getToken(f, token, "")) {
		//cout << token << endl;
		if (token == "vertex") break;
	}
	getToken(f, token, "");
	numVertices = atoi(token.c_str());
    
	// get vertex attributes
	while (getToken(f, token, "")) {
		if (token == "property") {
			getToken(f, t, ""); // token should be "float"
			getToken(f, t, ""); // attribute name
			attributes.push_back(t);
		}
		else if (token == "face") {
			getToken(f, t, "");
			numFaces = atoi(t.c_str());
			break;
		}
	}
    
	// read to end of header
	while (getToken(f, token, "")) {
		if (token == "end_header") break;
	}
    
	// get vertices
	float val;
	for (int i = 0; i < (int)(numVertices*attributes.size()); i++) {
		//getToken(f, token, "");
		//vertexData.push_back((float)atof(token.c_str()));
		fscanf(f, "%f", &val);
		vertexData.push_back(val);
	}
    
	// divide color values by 255, and flip normal directions if needed
	// This deals with issues related to exporting from Blender to ply
	// usin the y-axis as UP and the z-axis as FRONT.
	for (int i = 0; i < (int)attributes.size(); i++) {
		if (attributes[i] == "red" || attributes[i] == "green" || attributes[i] == "blue") {
			for (int j = 0; j < numVertices; j++) {
				vertexData[i + j*attributes.size()] /= 255.0f;
			}
		}
		else if (flipZ && (attributes[i] == "z" || attributes[i] == "nz")) {
			for (int j = 0; j < numVertices; j++) {
				vertexData[i + j*attributes.size()] *= -1.0f;
			}
		}
	}
    
	// get faces
	int idx;
	for (int i = 0; i < numFaces; i++) {
		faceIndices.clear();
		getToken(f, token, "");
		int numVerts = atoi(token.c_str());
		for (int j = 0; j < numVerts; j++) { // get all vertices in face
			//getToken(f, t, "");
			//faceIndices.push_back(atoi(t.c_str()));
			fscanf(f, "%d", &idx);
			faceIndices.push_back(idx);
		}
		for (int j = 2; j < numVerts; j++) { // make triangle fan
			indices.push_back(faceIndices[0]);
			indices.push_back(faceIndices[j-1]);
			indices.push_back(faceIndices[j]);
			numTriangles++;
		}
	}
	numIndices = (int)indices.size();
    
	//printf("vertices:%d, triangles:%d, attributes:%d\n",
	//	vertexData.size()/attributes.size(),
	//	indices.size()/3,
	//	attributes.size());
    
	fclose(f);
	return true;
}

//-------------------------------------------------------------------------//

void Material::bindMaterial(Transform &T, Camera &camera)
{
	glUseProgram(shaderProgram);

	// MATRICES FROM TRANSFORM
	GLint loc = glGetUniformLocation(shaderProgram, "uObjectWorldM");
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(T.transform));
	//
	loc = glGetUniformLocation(shaderProgram, "uObjectWorldInverseM");
    //printMat(T.invTransform);
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(T.invTransform));
	//
	glm::mat4x4 objectWorldViewPerspect = camera.worldViewProject * T.transform;
	loc = glGetUniformLocation(shaderProgram, "uObjectPerpsectM");
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(objectWorldViewPerspect));

	// MATERIAL COLORS
	for (int i = 0; i < (int) colors.size(); i++) {
		if (colors[i].id == -1) {
			loc = glGetUniformLocation(shaderProgram, colors[i].name.c_str());
			colors[i].id = loc;
		}
		if (colors[i].id >= 0) {
			glUniform4fv(colors[i].id, 1, &colors[i].val[0]);
		}
	}
    
    loc = glGetUniformLocation(shaderProgram, "uViewPosition");
    if(loc != -1)
    {
        glm::vec4 positionVec(camera.center, 0);
        glUniform4fv(loc, 1, &positionVec[0]);
    }
    
    loc = glGetUniformLocation(shaderProgram, "uViewDirection");
    if(loc != -1)
    {
        glm::vec4 directionVec(glm::normalize(camera.eye - camera.center), 0);
        glUniform4fv(loc, 1, &directionVec[0]);
    }
    

	// MATERIAL TEXTURES
	for (int i = 0; i < (int) textures.size(); i++) {
		if (textures[i].id == -1) {
			loc = glGetUniformLocation(shaderProgram, textures[i].name.c_str());
			textures[i].id = loc;
		}
		if (textures[i].id >= 0) {
			//printf("\n%d %d\n", textures[i].id, textures[i].val->samplerId);
			glActiveTexture(GL_TEXTURE0 + i);
			glUniform1i(textures[i].id, i);
			glBindTexture(GL_TEXTURE_2D, textures[i].val->textureId);
			glBindSampler(textures[i].id, textures[i].val->samplerId);
		}
	}
}
//-------------------------------------------------------------------------//

#define V_POSITION 0
#define V_NORMAL 1
#define V_ST 2
#define V_COLOR 3
int NUM_COMPONENTS[] = { 3, 3, 2, 3 };

bool TriMesh::sendToOpenGL(void)
{
	// Create vertex array object.  The vertex array object
	// holds the structure of how the vertices are stored. VAOs
	// also are bound for rendering.
	//
	glGenVertexArrays(1, &vao); // generate 1 array
	glBindVertexArray(vao);
    
	// Make and bind the vertex buffer object.  The vbo
	// holds the raw data that will be indexed by the vao.
	//
	GLuint vbo; // vertex buffer object
	glGenBuffers(1, &vbo); // generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(float), &vertexData[0], GL_STATIC_DRAW);
    
	// At this point, we have to tell the vertex array what kind
	// of data it holds, and where it is located in the vertex buffer.
	// The code here uses the first field of four possible data types
	// (position, normal, textureCoordinate, color)
	//
	int stride = (int)attributes.size() * sizeof(float); // size of a vertex in bytes
    
	for (int i = 0; i < (int)attributes.size(); i++) {
		int bindIndex = -1;
		//int numComponents = 0;
        
		if (attributes[i] == "x") bindIndex = V_POSITION;
		else if (attributes[i] == "nx") bindIndex = V_NORMAL;
		else if (attributes[i] == "s") bindIndex = V_ST;
		else if (attributes[i] == "red") bindIndex = V_COLOR;
        
		if (bindIndex >= 0) {
			//printf("bindIndex = %d\n", bindIndex);
			glEnableVertexAttribArray(bindIndex);
			glVertexAttribPointer(bindIndex, NUM_COMPONENTS[bindIndex],
                                  GL_FLOAT, GL_FALSE, stride, (void*)(i * sizeof(float)));
		}
	}
    
	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
    
	// Generate the index buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int),
                 &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind
    
	return true;
}

//-------------------------------------------------------------------------//

void TriMesh::draw(void)
{
	glBindVertexArray(vao); // bind the vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // bind the indices
    
	// draw the triangles.  modes: GL_TRIANGLES, GL_LINES, GL_POINTS
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)0);
}

//-------------------------------------------------------------------------//

TriMeshInstance::TriMeshInstance(void)
{
	triMesh = NULL;
	
	T.scale = glm::vec3(1, 1, 1);
	T.translation = glm::vec3(0, 0, 0);
}

//-------------------------------------------------------------------------//

void TriMeshInstance::draw(Camera &camera, Transform &t)
{
    T.refreshTransform();
    
    Transform tempTransform;
    tempTransform.transform = t.transform;
    

    switch(meshType)
    {
        case MESH:
        {  // I don't remember curly braces around switch cases being a
           // part of the C language, do you?
    
            mat.bindMaterial(t, camera);
            if (triMesh != NULL)
            {
                triMesh->draw();
            }
            else printf("Error! Null Mesh.");
            break;
        }
        case BILLBOARD:
        {
            glm::vec3 Q;
            Q = glm::normalize(camera.eye - t.translation);
            
            float theta = atan2(Q.x, Q.z);
            glm::vec3 axis = {0,1,0};
            
            
            glm::mat4x4 R = glm::axisAngleMatrix(axis, theta);
            
            t.transform = t.transform * R ;
            
            mat.bindMaterial(t, camera);
            if (triMesh != NULL)
            {
                triMesh->draw();
            }
            else printf("Error! Null Mesh.");
            
            break;
        }
        case POINT_SPRITE:
        {
            glm::vec3 Q;
            Q = glm::normalize(camera.eye - camera.center);
            
            float theta = atan2(Q.x, Q.z);
            float phi = asin(Q.y);
            glm::vec3 axis = {0, 1,0};
            
            
            glm::mat4x4 R = glm::axisAngleMatrix(axis, theta);
            
            axis = {-1, 0, 0};
            glm::mat4x4 Rup = glm::axisAngleMatrix(axis, phi);
            
            t.transform = t.transform * R * Rup;
            
            mat.bindMaterial(t, camera);
            
            if(triMesh != NULL)
            {
                triMesh->draw();
            }
            else
            {
                printf("Error! Null Mesh.");
            }
            break;
        }
            
        default:
            break;
    }
}
//-------------------------------------------------------------------------//

GLuint gLightBufferObject = NULL_HANDLE;
int gNumLights = 0;
Light gLights[MAX_LIGHTS];

void initLightBuffer() 
{
	if (gLightBufferObject != NULL_HANDLE) return;
	glGenBuffers(LIGHT_BUFFER_ID, &gLightBufferObject); // create a new buffer id

	glBindBuffer(GL_UNIFORM_BUFFER, gLightBufferObject); // bind the new buffer
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * MAX_LIGHTS, gLights, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0); // unbind buffer
}
//-------------------------------------------------------------------------//
