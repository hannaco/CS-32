void listAll(string path, const Class* c)  // two-parameter overload
{
    path += c->name();
    cout << path << endl;
    vector<Class*>::const_iterator it;
    it = c->subclasses().begin();
    path += "=>";
    while (it != c->subclasses().end())
    {
        listAll(path, *it);
        it++;
    }
}