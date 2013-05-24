class TimeZone {
public:
   TimeZone();
   TimeZone(QString aName, QString aDescription, QString anOffset, int anOffsetDirection, int anOffsetHours, int anOffsetMinutes);
   QString name, description, offset;
   int offsetDirection, offsetHours, offsetMinutes;
};
