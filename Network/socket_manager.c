typedef struct {
  // linked_list
  // rw_lock

  // socket
  // socket_constructor????

  // socket_manager
  //  receive
      // lock + socket->receive
  // send
    // lock + socket->send
  // get_port
    // lock + socket->get_port
  // add_socket
  // remove_socket
  // destroy

} socket_manager_t;


// PLUGIN_CALLBACK (socket_register, socket_default_socket_create )

// socket_register ( plugin, feature, reg, (socket_constructor_t) data)
// charon->socket->add_socket( charon->socket, data);