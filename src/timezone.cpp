class TimeZone {
public:
   TimeZone()
   {
        description = QString("");
        name = QString("");
        offset = QString("");
        offsetDirection = 0;
        offsetHours = 0;
        offsetMinutes = 0;
   }
   TimeZone(QString aName, QString aDescription, QString anOffset, int anOffsetDirection, int anOffsetHours, int anOffsetMinutes)
   {
        description = aDescription;
        name = aName;
        offset = anOffset;
        offsetDirection = anOffsetDirection;
        offsetHours = anOffsetHours;
        offsetMinutes = anOffsetMinutes;
   }
   QString name, description, offset;
   int offsetDirection, offsetHours, offsetMinutes;
};