#include "InterServerImpl.h"
#include "Logging.h"

InterServerImpl::InterServerImpl() {
}

User InterServerImpl::retrieveUser(const string &id, const Ice::Current&) {
	INFO("<stub> retrieveUser ", id);
	User *u = new User();
	return *u;
}

void InterServerImpl::invite(const User &participant, const string &chat, const ByteSeq&, const Ice::Current&) {
	INFO("<stub> invite ", participant.ID, " to ", chat);
}

void InterServerImpl::sendMessage(const User &sender, const ByteSeq&, const string &chat, const Ice::Current&) {
	INFO("<stub> sendMessage from ", sender.ID, " to ", chat);
}

void InterServerImpl::leaveChat(const User &participant, const string &chat, const Ice::Current&) {
	INFO("<stub> leaveChat ", participant.ID, " leaving ", chat);
}

void InterServerImpl::clientInitChat(const User &recipient, const StringSeq&, const string &chat, const ByteSeq&, const Ice::Current&) {
	INFO("<stub> clientInitChat notify ", recipient.ID, " of ", chat);
}

void InterServerImpl::clientAddChatParticipant(const User &recipient, const User &other, const string &chat, const Ice::Current&) {
	INFO("<stub> clientAddChatParticipant notify ", recipient.ID, " that ", other.ID, " joined ", chat);
}

void InterServerImpl::clientRemoveChatParticipant(const User &recipient, const User &other, const string &chat, const Ice::Current&) {
	INFO("<stub> clientRemoveChatParticipant notify ", recipient.ID, " that ", other.ID, " left ", chat);
}

void InterServerImpl::clientAppendMessageToChat(const User &recipient, const ByteSeq&, const string &chat, const User &sender, const Ice::Current&) {
	INFO("<stub> clientAppendMessageToChat notify ", recipient.ID, " that ", sender.ID, " sent a message to ", chat);
}
