#include "ProfileTest.h"
#include "../src/utils/utils.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ProfileTest);

ProfileTest::ProfileTest() {
}

void ProfileTest::setUp() {
}

void ProfileTest::getEmail() {
	const string mail = "prueba@mail.com";
	Profile profile(mail);
	CPPUNIT_ASSERT_EQUAL(mail, profile.getEmail());
}

void ProfileTest::profileToJSON() {
	const string mail = "prueba@mail.com";
	Profile profile(mail);
	Json::Value jsonProfile = profile.profileToJSON();
	CPPUNIT_ASSERT_EQUAL(jsonProfile.get("email", "").asString(), profile.getEmail());
}

void ProfileTest::addRecommendation() {
	const string mail = "prueba@mail.com";
	Profile profile(mail);
	const string mail2 = "prueba2@mail.com";
	Profile profile2(mail2);
	int prevRecommendations = profile.getRecommendationsCount();
	profile.addRecommendation(profile2.getEmail());
	CPPUNIT_ASSERT_EQUAL(prevRecommendations + 1, profile.getRecommendationsCount());
}


void ProfileTest::manageInvitations() {
	const string mail = "prueba@mail.com";
	Profile profile(mail);
	const string mail2 = "prueba2@mail.com";
	Profile profile2(mail2);
	int prevInvitations = profile.getInvitationsSent().size();
	profile.addSentInvitation(profile2.getEmail());
	int postInvitations = profile.getInvitationsSent().size();
	int prevInvitationsRec = profile2.getInvitationsReceived().size();
	profile2.addReceivedInvitation(profile2.getEmail());
	int postInvitationsRec = profile2.getInvitationsReceived().size();
	CPPUNIT_ASSERT_EQUAL(prevInvitations + 1, postInvitations);
	CPPUNIT_ASSERT_EQUAL(prevInvitationsRec + 1, postInvitationsRec);
}

void ProfileTest::verifySkills() {
	Json::Value jsonProfile;
	string newSkill = "skill1";
	jsonProfile["email"] = "myemail@email.com";
	jsonProfile["first_name"] = "first_name";
	jsonProfile["last_name"] = "last_name";
	jsonProfile["about"] = "about";
	jsonProfile["pic"] = "pic";
	jsonProfile["job_position"] = "job_position";
	jsonProfile["location"]["latitude"] = 10;
	jsonProfile["location"]["longitude"] = 10;
	jsonProfile["skills"].append(newSkill);
	jsonProfile["contacts"] = Json::arrayValue;
	jsonProfile["invitationsSent"] = Json::arrayValue;
	jsonProfile["invitationsReceived"] = Json::arrayValue;
	jsonProfile["recommendations"]["users"] = Json::arrayValue;
	Profile p(jsonProfile);
	std::set<string> skills = p.getSkills();
	CPPUNIT_ASSERT(utils::setContainsValue(skills,newSkill));
}

void ProfileTest::correctProfileConversion(){
	string mail = "usuariogenerico@mail.com";
	string first_name = "Usuario";
	string last_name = "Generico";
	string p_pic = "qwertyuixcvbnmsdfghjk";
	string job_position = "desarrollador";
	double latitude = 0;
	double longitude = 0;
	Profile p(mail,first_name,last_name,p_pic,job_position,latitude,longitude);
	Json::Value jsonProfile = p.publicProfileToJSON();
	CPPUNIT_ASSERT_EQUAL(jsonProfile.get("email","").asString(), mail);
	CPPUNIT_ASSERT_EQUAL(jsonProfile.get("first_name","").asString(), first_name);
	CPPUNIT_ASSERT_EQUAL(jsonProfile.get("last_name","").asString(), last_name);
	CPPUNIT_ASSERT_EQUAL(jsonProfile.get("pic","").asString(), p_pic);
	CPPUNIT_ASSERT_EQUAL(jsonProfile.get("job_position","").asString(), job_position);
	double lat = jsonProfile["location"]["latitude"].asDouble();
	double lon = jsonProfile["location"]["longitude"].asDouble();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(lon, 0, 0.1);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(lat, 0, 0.1);
}


void ProfileTest::tearDown() {

}

ProfileTest::~ProfileTest() {

}
