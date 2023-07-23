# USocial - Social Network System

USocial is a C++ application that simulates a simple social network. Users can interact with each other by adding friends, sending messages, and sharing posts. Additionally, the system supports business users, who have the capability to send messages to all users, regardless of their friendship status.

## Features

1. **Users**: Each user in USocial has a unique ID, a name, and a list of friends. Users can add and remove friends, post messages, and view posts from their friends.

2. **Messages**: Users can send and receive messages. Regular users can only send messages to their friends, while business users can send messages to all users in the network.

3. **Posts**: Users can create posts containing text and optional media (photo, audio, or video). Posts are visible to the user's friends.

4. **Business Users**: Business users have additional privileges, allowing them to send messages to all users, not just their friends.

## Class Structure

USocial consists of the following classes:

1. **User**: Represents a regular user in the social network.

2. **BusinessUser**: Represents a business user with extended privileges. Inherits from the `User` class.

3. **Post**: Represents a post made by a user. Contains text and optional media.

4. **Message**: Represents a message sent between users.

5. **Media**: An abstract class representing media that can be attached to posts. Contains pure virtual functions to be implemented by derived classes (`Photo`, `Audio`, and `Video`).

## Exception Handling

USocial supports two custom exceptions:

1. **NotAFriendException**: This exception is thrown when a user attempts to send a message to a user who is not on their friends list.

2. **FriendAlreadyExistsException**: This exception is thrown when a user attempts to add a friend who is already on their friends list.

## Credits
Author: Ben Gabay