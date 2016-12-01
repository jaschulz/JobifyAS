#Jobify

# **AppServer** #

## Building

**Building AppServer**

1. cd AppServer/

2. sudo sh install.sh

install.sh installs every dependency so that the AppServer can be built and run flawlessly.

## REST API

### Objects

#### User
>```
>{
>    email: string,
>    first_name: string,
>    last_name: string,
>    about: string,
>    profile_pic: string (url),
>    skills: [SKILLS],
>    job_position: JOB_POSITION,
>    location: LOCATION,
>    experiencies: [EXPERIENCES],
>    contacts: [CONTACTS]
>}
>```

#### Contact
>```
>{
>    email: string,
>    first_name: string,
>    last_name: string,
>    profile_pic: string (url),
>    job_position: JOB_POSITION
>}
>```

#### Skill
>```
>{
>    name: string,
>    description: string,
>    category: string
>}
>```

#### Location
>```
>{
>    latitude: double,
>    longitude: double
>}
>```

#### Experience
>```
>{
>    id: integer,
>    where: string,
>    job_position: JOB_POSITION
>}
>```

#### Job position
>```
>{
>    name: string,
>    description: string,
>    category: string
>}
>```

#### Chat
>```
>{
>    id: integer,
>    contact: USER,
>    last_message: string
>}
>```

#### Message
>```
>{
>    id: integer,
>    sender: USER,
>    receiver: USER,
>    text: string,
>    time: time
>}
>```


---

### HTTP Requests

#### User registration
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>POST</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users</td>
    </tr>
    <tr>
      <td>Parameters</td>
      <td><ul><li>email (string)</li><li>password (string)</li></td>
    </tr>
  </tbody>
</table>
>Response (HTTP 200 OK):
>```
>{
>    token: string
>}
>```
>Response (HTTP 401 Unauthorized):
>```
>{
>    error: string
>}
>```


#### User login
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>POST</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/session</td>
    </tr>
    <tr>
      <td>Parameters</td>
      <td><ul><li>email (string)</li><li>password (string)</li></td>
    </tr>
    <tr>
      <td>Parameters (Facebook login)</td>
      <td><ul><li>facebook_token (string)</li></td>
    </tr>
  </tbody>
</table>
>Response (HTTP 200 OK):
>```
>{
>    token: string,
>    user: USER
>}
>```
>Response (HTTP 401 Unauthorized):
>```
>{
>    error: string
>}
>```

#### User's profile
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>GET</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users/:id</td>
    </tr>
    <tr>
        <td>Requires token?</td>
        <td>Yes</td>
    </tr>
  </tbody>
</table>
>Response (HTTP 200 OK):
>```
>{
>    user: USER
>}
>```

#### User's profile edit
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>PUT</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users/:id</td>
    </tr>
    <tr>
      <td>Requires token?</td>
      <td>Yes</td>
    </tr>
    <tr>
        <td>Parameters (Optionals)</td>
        <td><ul>
            <li>first_name (string)</li>
            <li>last_name (string)</li>
            <li>about_me (string)</li>
            <li>profile_pic (file)</li>
            <li>skills (skills)</li>
            <li>location (location)</li>
            <li>experiences (experiences)</li>
    </tr>
  </tbody>
</table>
>Response (HTTP 200 OK):
>```
>{
>    user: USER
>}
>```


#### Contacts search
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>GET</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users</td>
    </tr>
    <tr>
      <td>Parameters</td>
      <td>
        <ul>
         <li>range (double)</li>
         <li>skills (list of skills)</li>
         <li>job_position (job position)</li>
         <li>user (string, substring of user's email or last name) </li>
        </ul>
      </td>
    </tr>
  </tbody>
</table>
>Response (HTTP 200 OK):
>```
>{
>    users: [USERS]
>}
>```


#### Send Invitation
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>POST</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users/:id/contacts</td>
    </tr>
    <tr>
      <td>Requires token?</td>
      <td>Yes</td>
    </tr>
    <tr>
      <td>Parameters</td>
      <td>
        <ul>
          <li>email (string)</li>
        </ul>
      </td>
    </tr>
  </tbody>
</table>
>No response
>Server status 201 on success
>Server status 404 if contact does not exist

#### User contacts
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>GET</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users/:id/contacts</td>
    </tr>
    <tr>
        <td>Requires token?</td>
        <td>Yes</td>
    </tr>
  </tbody>
</table>
>Response (HTTP 200 OK):
>```
>{
>    contacts: [USERS]
>}
>```


#### Chats' list
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>GET</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users/:id/chats</td>
    </tr>
    <tr>
      <td>Requires token?</td>
      <td>Yes</td>
    </tr>
  </tbody>
</table>
>Response (HTTP 200 OK):
>```
>{
>    chats: [CHATS]
>}
>```

#### Specific chat
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>GET</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users/:user_id/chats/:chat_id</td>
    </tr>
    <tr>
      <td>Requires token?</td>
      <td>Yes</td>
    </tr>
  </tbody>
</table>
>Response (HTTP 200 OK):
>```
>{
>    chat: CHAT,
>    messages: [MESSAGES]
>}
>```

#### Send message
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>POST</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users/:user_id/chats/:chat_id/message</td>
    </tr>
    <tr>
        <td>Requires token?</td>
        <td>Yes</td>
    </tr>
    <tr>
      <td>Parameters</td>
      <td>
        <ul>
         <li>message (string)</li>
        </ul>
      </td>
    </tr>
  </tbody>
</table>
>Response: HTTP 204 No response


#### Update user's location
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>POST</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users/:id/location</td>
    </tr>
    <tr>
        <td>Requires token?</td>
        <td>Yes</td>
    </tr>
    <tr>
      <td>Parameters</td>
      <td>
        <ul>
         <li>latitude (double)</li>
         <li>longitude (double)</li>
        </ul>
      </td>
    </tr>
    <tr>
      <td>Requires token?</td>
      <td>Yes</td>
    </tr>
  </tbody>
</table>
>Response: HTTP 204 No response

#### Recommend User
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>POST</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/users/:id/recommendation</td>
    </tr>
    <tr>
        <td>Requires token?</td>
        <td>Yes</td>
    </tr>
  </tbody>
</table>
>Response: HTTP 204, HTTP 401 

#### User's Ranking
><table>
  <tbody>
    <tr>
      <th>Attribute</th>
      <th>Value</th>
    </tr>
    <tr>
      <td>Method</td>
      <td>GET</td>
    </tr>
    <tr>
      <td>URL</td>
      <td>/ranking</td>
    </tr>
    <tr>
        <td>Parameters (Optionals)</td>
        <td><ul>
            <li>skills</li>
            <li>job_position</li>
    </tr>
  </tbody>
</table>
>Response (HTTP 200 OK):
>```
>{
>    users: [USERS]
>}
>```

