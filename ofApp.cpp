#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(1);
	ofEnableDepthTest();

	this->font_size = 55;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum());
	ofRotateX(ofGetFrameNum());

	string word = "0123456789";
	int sample_count = 30;
	auto base_location = glm::vec3(0, 0, 0);

	for (int r = 0; r < 80; r++) {

		auto radius = 150 - r * 1;

		for (int i = 0; i < 3; i++) {

			auto noise_param = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

			auto angle_x = ofMap(ofNoise(noise_param.x, ofGetFrameNum() * 0.001), 0, 1, -PI * 1.5, PI * 1.5);
			auto angle_y = ofMap(ofNoise(noise_param.y, ofGetFrameNum() * 0.001), 0, 1, -PI * 1.5, PI * 1.5);
			auto angle_z = ofMap(ofNoise(noise_param.z, ofGetFrameNum() * 0.001), 0, 1, -PI * 1.5, PI * 1.5);

			auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

			auto rotation = glm::rotate(glm::mat4(), (i * 7.2f) * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
			auto noise_location = glm::vec4(this->font_size * 0.5, base_location.y + this->font_size * -0.5, radius, 0) * rotation;
			noise_location = noise_location * rotation_z * rotation_y * rotation_x;

			int word_index = (int)ofMap(ofNoise(noise_location.x * 0.001, noise_location.y * 0.001, noise_location.z * 0.001, ofGetFrameNum() * 0.008), 0, 1, 0, word.size() * 2) % word.size();

			ofPath chara_path = this->font.getCharacterAsPoints(word[word_index], true, false);
			vector<ofPolyline> outline = chara_path.getOutline();

			ofFill();
			ofSetColor(39);
			ofBeginShape();
			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

				if (outline_index != 0) { ofNextContour(true); }

				auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();
				for (auto& vertex : vertices) {

					auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 350);
					auto rotation = glm::rotate(glm::mat4(), (i * 7.2f) * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
					location = glm::vec4(location, 0) * rotation;
					location += base_location;
					location = glm::vec4(location, 0) * rotation_z * rotation_y * rotation_x;

					location = glm::normalize(location) * radius;

					ofVertex(location);
				}
			}
			ofEndShape(true);

			ofNoFill();
			ofSetColor(239);
			ofBeginShape();
			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

				if (outline_index != 0) { ofNextContour(true); }

				auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();
				for (auto& vertex : vertices) {

					auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 350);
					auto rotation = glm::rotate(glm::mat4(), (i * 7.2f) * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
					location = glm::vec4(location, 0) * rotation;
					location += base_location;
					location = glm::vec4(location, 0) * rotation_z * rotation_y * rotation_x;

					location = glm::normalize(location) * radius;

					ofVertex(location);
				}
			}
			ofEndShape(true);
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}