#include "HomieNode.hpp"

using namespace HomieInternals;

HomieNode::HomieNode(const char* id, const char* type, NodeInputHandler inputHandler, bool subscribeToAll, char Gpio)
: _id(id)
, _type(type)
, _subscriptionsCount(0)
, _subscribeToAll(subscribeToAll)
, _inputHandler(inputHandler) {
  if (strlen(id) + 1 > MAX_NODE_ID_LENGTH || strlen(type) + 1 > MAX_NODE_TYPE_LENGTH) {
    Serial.println(F("✖ HomieNode(): either the id or type string is too long"));
    abort();
  }

  this->_id = id;
  this->_type = type;
  this->_gpio = Gpio;
}

bool HomieNode::InitNodePin(const char* id, char Gpio) {
	if (Gpio == 'z')
		return false ;
	if ((strcmp(this->_type, "light") == 0) ||
			(strcmp(this->_type, "switch") == 0)) {
		  pinMode(Gpio, OUTPUT);
		  digitalWrite(Gpio, LOW);
		 Serial.print(F("NBM Inited OP pin: "));
		 Serial.println(Gpio,DEC);
	} else {
		if ((strcmp(this->_type, "pwm") == 0) ||
				(strcmp(this->_type, "dimmer") == 0)) {
			return false; /*TODO:PWM here */
		} else {
			return false;
		}
	}
}

void HomieNode::subscribe(const char* property, PropertyInputHandler inputHandler) {
  if (strlen(property) + 1 > MAX_NODE_PROPERTY_LENGTH) {
    Serial.println(F("✖ subscribe(): the property string is too long"));
    abort();
  }

  if (this->_subscriptionsCount > MAX_SUBSCRIPTIONS_COUNT_PER_NODE) {
    Serial.println(F("✖ subscribe(): the max subscription count has been reached"));
    abort();
  }

  Subscription subscription;
  strcpy(subscription.property, property);
  subscription.inputHandler = inputHandler;
//  subscription.homenode = &this;
  this->_subscriptions[this->_subscriptionsCount++] = subscription;
}

bool HomieNode::InInputHandler(String property, String value, char *ptype) const
{
	const char * type = NULL;

	type = this->getType();
	strcpy(ptype,type);
	Serial.println(F("NBM In input handler"));
	if (type==NULL)
		Serial.println(F("NBM type not found"));
	else {
		Serial.println(F("NBM type found"));
		Serial.println(type);
	}

	char ppin = this->getPin();
	if (ppin == 'z')
		return false;


	int pin = (int)ppin;
	if ((strcmp(type,"light") == 0) || (strcmp(type, "switch") == 0))
		goto lightcontrol;
	if ((strcmp(type,"pwm") == 0) || (strcmp(type, "dimmer") == 0))
		goto pwmcontrol;
	else
		return false;

lightcontrol:
	if ((property == "on") && (value == "true")) {
		digitalWrite(pin, HIGH);
	} else if ((property == "on") && (value == "false")) {
		digitalWrite(pin, LOW);
	} else
		return false;

pwmcontrol:
	if ((property == "value") && ((value.toInt() >= 0) &&
				      (value.toInt() < 256))) {
		analogWrite(pin, value.toInt());
	}
	return true;
}

const char* HomieNode::getId() const {
  return this->_id;
}

const char* HomieNode::getType() const {
  return this->_type;
}

char HomieNode::getPin() const {
  return this->_gpio;
}
const Subscription* HomieNode::getSubscriptions() const {
  return this->_subscriptions;
}

unsigned char HomieNode::getSubscriptionsCount() const {
  return this->_subscriptionsCount;
}

bool HomieNode::getSubscribeToAll() const {
  return this->_subscribeToAll;
}

NodeInputHandler HomieNode::getInputHandler() const {
  return this->_inputHandler;
}
