void decode_huff(node * root, string s) {
    int index = 0;
    string decoded = "";
    while(index < s.length())
    {
        node* traverse = root;
        node* traverse_behind = root;
        while(traverse != NULL)
        {
            if(s[index] == '0') traverse = traverse->left;
            else traverse = traverse->right;
            if(traverse == NULL)
            {
                decoded.push_back(traverse_behind->data);
                index--;
            }
            else 
            {
                traverse_behind = traverse;
            }
            index++;
        }
    }
    cout << decoded << endl;
}
