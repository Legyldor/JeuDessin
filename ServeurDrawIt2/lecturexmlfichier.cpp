#include "lecturexmlfichier.h"

LectureXMLFichier::LectureXMLFichier(QObject *parent) : QObject(parent)
{

}

LectureXMLFichier::~LectureXMLFichier()
{

}

QList<QString>* LectureXMLFichier::lectureXML(){
    QList<QString> *listeMots = new QList<QString>();
    qDebug() << "LectureXMLFichier::lectureXML()";
    QString fileName = "..//BDD.xml";
    QFile file(fileName);
    // Ouverture du fichier
    file.open(QFile::ReadOnly | QFile::Text);
    if(file.isOpen()){

        QDomDocument doc;
        // Ajoute le contenu un QDomDocument
        doc.setContent(&file, false);

        // racine = root
        QDomElement racine = doc.documentElement();

        racine = racine.firstChildElement();

        //Boucle permettant la navigation dans le fichier XML
        while(!racine.isNull())
        {
            if(racine.tagName() == "mot")
            {

                // Récupération de l'attribut nom de l'élément <mot>
                QString attNom = racine.attribute("nom");
                listeMots->append(attNom);
            }

            // On va à l'élément mot suivant
            racine = racine.nextSiblingElement();
        }

    } else {
        qDebug() << "Erreur de lecture du fichier :"+ file.fileName() +" Erreur :"+file.errorString();
    }

    return listeMots;
}


