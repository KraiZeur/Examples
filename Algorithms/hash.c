// https://stackoverflow.com/questions/7666509/hash-function-for-string

unsigned long
hash_string(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/*

network* net = init();
net->send("127.0.0.1", 1598);
      ->get_hashed_socket();
         ->find_from_list
      return NULL or socket*
return NULL if NULL
else



*/

