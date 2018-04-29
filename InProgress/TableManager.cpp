/*

L'idée est d'avoir un Manager de Table par identifiant
Chaque table possède un nombre de champs dynmaique du moins à l'initialisation.

Pour un exemple simpliste :

Table* tableClassification = new Table("TableClassification");
tableClassification.addEntry(new Entry<std::string>("ServiceName"));
tableClassification.addEntry(new Entry<Integer>("Classification"));

TableManager* manager = new TableManager();
manager.register(tableClassification);


Table* tbl=manager->getTable("TableClassification");

Entry* entry=tbl->getEntry("SVC_IMG");



*/
#include <string>

class Table {
private:
public:
    explicit Table(std::string name)
    {

    }
};

