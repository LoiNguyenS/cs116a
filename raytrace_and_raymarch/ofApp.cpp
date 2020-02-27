#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(ofColor::black);
	bHide = false;
	image.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR); //allocates image width and height
	mainCam.setDistance(20);
	mainCam.setNearClip(.1);
	theCam = &mainCam;
	sideCam.setPosition(60, 0, 0);
	sideCam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));

	previewCam.setPosition(0, 0, 10); //same position as render cam

	//texture3.load("images/wood.jpg");
	//inserts spheres and plane to scene object vector
	
	//scene.push_back(new Sphere(glm::vec3(0, 0, 0), 1, ofColor::red));
	scene.push_back(new Sphere(glm::vec3(2.0f, 0.0f,-3.0f), 1, ofColor::blue));
	//scene.push_back(new Sphere(glm::vec3(-1, 0, 2), 1, ofColor::blue));
	//scene.push_back(new Sphere(glm::vec3(0, 1.5, 1), 1, ofColor::green));
	scene.push_back(new Torus(glm::vec3(1.0f, 1.0f, 0.0f), 1.0,0.5, ofColor::red));

	scene.push_back(new Plane(glm::vec3(0, -2.0, 0), glm::vec3(0, 1, 0), ofColor::lightGray, 20, 20));
	//insert lights into light vector
	lights.push_back(new Light(glm::vec3(-10.0f, 6.0f, 8.0f), .1, ofColor::yellow, 300));
	lights.push_back(new Light(glm::vec3(0, 10, 8), .1, ofColor::yellow, 450.0));
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	theCam->begin();

	// draw simple box object
	//
	ofNoFill();
	//ofDrawBox(30);
	mainCam.draw();

	//draws 3 differen spheres in idfferent colors and 1 plane
	for (int w = 0; w < scene.size(); w++)
	{
		ofSetColor(scene[w]->diffuseColor);
		scene[w]->draw();
	}
	ofSetColor(ofColor::yellow);
	for (int w = 0; w < lights.size(); w++)
	{
		ofSetColor(lights[w]->diffuseColor);
		lights[w]->draw();
	}




	//draws the viewplane
	ofSetColor(ofColor::white);
	renderCam.view.draw();

	theCam->end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'C':
	case 'c':
		if (mainCam.getMouseInputEnabled()) mainCam.disableMouseInput();
		else mainCam.enableMouseInput();
		break;
	case 'F':
	case 'b':
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	case 'h':
		bHide = !bHide;
		break;
	case OF_KEY_F1:
		theCam = &mainCam;
		break;
	case OF_KEY_F2:
		theCam = &sideCam;
		break;
	case OF_KEY_F3:
		theCam = &previewCam;
		break;
		//ray tracing
	case 'r':
	case 'R':
		rayTrace();
		break;
	case 'm':
		rayMarch();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
bool Plane::intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normalAtIntersect)
{
	float dist;
	bool insidePlane = false;
	bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
	if (hit)
	{
		Ray r = ray;
		point = r.evalPoint(dist);
		normalAtIntersect = this->normal;
		glm::vec2 xrange = glm::vec2(position.x - width / 2, position.x + width / 2);
		glm::vec2 zrange = glm::vec2(position.z - height / 2, position.z + height / 2);
		if (point.x < xrange[1] && point.x > xrange[0] && point.z < zrange[1] && point.z > zrange[0])
		{
			insidePlane = true;
		}
	}
	return insidePlane;
}
// Convert (u, v) to (x, y, z) 
// We assume u,v is in [0, 1]
//
glm::vec3 ViewPlane::toWorld(float u, float v) { //changes from camera's min and max to a 3 coordinate
	float w = width();
	float h = height();
	return (glm::vec3((u * w) + min.x, (v * h) + min.y, position.z));
}

// Get a ray from the current camera position to the (u, v) position on
// the ViewPlane
//
Ray RenderCam::getRay(float u, float v) {
	glm::vec3 pointOnPlane = view.toWorld(u, v);
	return(Ray(position, glm::normalize(pointOnPlane - position)));
}
void ofApp::rayTrace() //method for ray tracing
{

	ofSetColor(ofColor::white);

	glm::vec3 nor = glm::vec3(0, 0, 0); //a normal
	glm::vec3 aPoint = glm::vec3(0, 0, 0); //a point
	//loop to to do the ray tracing
	for (float i = 0; i < imageWidth; i++)
	{
		for (float j = 0; j < imageHeight; j++)
		{
			float camDistance = INFINITY;
			for (int k = 0; k < scene.size(); k++)
			{
				cout << "-";
				if (scene[k]->intersect(renderCam.getRay((i + 0.5) / imageWidth, (j + 0.5) / imageHeight), aPoint, nor) && glm::distance(glm::vec3(0, 0, 10), scene[k]->position) < camDistance)
				{
					ofColor aColor = phongAndLambert(aPoint, nor, scene[k]->diffuseColor, ofColor::white, 50);
					image.setColor(i, imageHeight - j, aColor);
					image.update();
					camDistance = glm::distance(glm::vec3(0, 0, 10), scene[k]->position);
				}
			}
		}
	}
	//saves image
	cout << "success" << endl;
	image.save("images/myImage1.jpg");

}
//		Ray shadowRay(p + glm::vec3(.1, .1, .1), normalize(someLight));//shadows = inShadow(shadowRay);
ofColor ofApp::phongAndLambert(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse, const ofColor specular, float power){	float specCo = 0.3f;	float diffuseCo = 0.4f;	float powerNum1 = 2.0F;	ofColor someColor1;	glm::vec3 biSec;	glm::vec3 camP = glm::vec3(0, 0, 10);	glm::vec3 rayD = camP - p;	float numD = 0;	float numS = 0;	bool shadows = false;	for (int o = 0; o < lights.size(); o++)	{		glm::vec3 someLight = lights[o]->position - p;		biSec = glm::normalize(someLight + rayD);		numD += (lights[o]->intensity / glm::pow(glm::distance(lights[o]->position, p), powerNum1)) * glm::max(0.0F, glm::dot(glm::normalize(norm), glm::normalize(someLight)));		numS += (lights[o]->intensity / glm::pow(glm::distance(lights[o]->position, p), powerNum1)) * glm::pow(glm::max(0.0F, glm::dot(glm::normalize(norm), biSec)), power);		someColor1 = (numD * diffuse * diffuseCo) + (specular * specCo * numS);	}	return someColor1;}
bool ofApp::inShadow(const Ray &r){	bool bShadow = false;	glm::vec3 aPoint1;	glm::vec3 nor1;	for (int m = 0; m < scene.size(); m++)	{		if (scene[m]->intersect(r, aPoint1, nor1))		{			bShadow = true;		}	}	return bShadow;}ofColor ofApp::getTexel(float i, float j){	ofColor theTexel;	float a, b, c, d;	a = b = c = d = 0;	a = ofMap(i, -10, 10, 0, 10);	a = fmodf(a, 1);	b = ofMap(j, -10, 10, 0, 10);	b = fmodf(b, 1);	c = a * texture3.getWidth() - 0.5;	d = b * texture3.getHeight() - 0.5;	theTexel = texture3.getColor(c, d);	return theTexel;}void ofApp::rayMarch() {
	renderCam.view.draw();
	ofSetColor(ofColor::white);
	glm::vec3 nor = glm::vec3(0, 0, 0); 
	glm::vec3 aPoint = glm::vec3(0, 0, 0);
	ofColor texel;
	ofColor aColor;
	bool hit = false;
	int num = 0;
	cout << "starting render\n";
	cout << imageWidth << " " << imageHeight << endl;
	float temp = 0.0f;
	for (float i = 0.0F; i < imageWidth; i++)
	{
		for (float j = 0.0F; j < imageHeight; j++)
		{
			glm::vec3 point(0,0,0);
			glm::vec3 camP = glm::vec3(0, 0, 10);			Ray rayD(camP,point);
			hit = rayBool(renderCam.getRay((i + 0.5) / imageWidth, (j + 0.5) / imageHeight), point,num);
			if (hit)
			{
				nor = getNormalRM(point);
				aColor = phongAndLambert(point, nor, scene[num]->diffuseColor, ofColor::white, 50.0f);
			}
			else
				aColor = ofColor::black;
			temp = imageHeight - j;
			image.setColor(i, temp, aColor);
			image.update();
			
		}
	}
	image.save("images/myImage16.png");	cout << "success" << endl;}bool ofApp::rayBool(Ray r, glm::vec3& p,int & k) {	bool hit = false;	p = r.p;	for (int i = 0; i < 20; i++) {		float dist = sceneSDF(p,k);		if (dist < 0.05)		{			hit = true;			break;		}		else if (dist > 50) {			break;		}		else			p = p + r.d*dist;	}	return hit;}float Sphere::sdf(const glm::vec3 & p) {	return glm::length(p-position) - radius;}float Plane::sdf(const glm::vec3 & p) {	return p.y - position.y;}float Torus::sdf(const glm::vec3 & p) {	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(position.x, position.y, position.z));	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));	glm::vec4 pos(p, 1.0f);	glm::vec4 newpos = trans * pos;	glm::vec3 pos1(newpos.x, newpos.y, newpos.z);	glm::vec2 temp(pos1.x, pos1.z);	glm::vec2 q = glm::vec2(glm::length(temp) - radius1, pos1.y);	return glm::length(q)-radius2;}float ofApp::sceneSDF(glm::vec3 p,int &n){
	float closestDistance = INFINITE;	for (int k = 0; k < scene.size(); k++)	{		float d = scene[k]->sdf(p);		if (d < closestDistance) {			closestDistance = d;			n = k;		}	}	return closestDistance;}glm::vec3 ofApp::getNormalRM(const glm::vec3 &p) {
	float eps = 0.01F;	int p2 = 0;	float dp = sceneSDF(p,p2);	glm::vec3 n(dp - sceneSDF(glm::vec3(p.x - eps, p.y, p.z),p2), dp - sceneSDF(glm::vec3(p.x, p.y - eps, p.z),p2), dp - sceneSDF(glm::vec3(p.x, p.y, p.z - eps),p2));	return glm::normalize(n);}void Torus::draw() {
	//glShadeModel(GL_FLAT);
	//glEnable(GL_DEPTH_TEST);
	/*
	ofSetColor(50, 50, 50);

	float i, j, k;
	float s, t, x, y, z, twopi;

	twopi = 2 * PI;
	for (i = 0; i < radius1; i++) {
		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= radius2; j++) {
			for (k = 1; k >= 0; k--) {
				s =fmodf((i + k),radius1 + 0.5);
				t = fmodf(j , radius2);
				//cout << " point:"<< s<< " ";  
				//cout << " point:"<< t<< " ";  
				x = (1 + .1*cos(s*twopi / radius1))*cos(t*twopi / radius2);
				y = (1 + .1*cos(s*twopi / radius1))*sin(t*twopi / radius2);
				z = .1 * sin(s * twopi / radius1);
				//cout << " vertex x:"<< x<< " ";		  
				glVertex3f(x, y, z);

			}
		}
		glEnd();
	}
	*/
}