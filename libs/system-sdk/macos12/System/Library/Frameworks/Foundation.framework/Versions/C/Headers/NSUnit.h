/*
 NSUnit.h
 Copyright (c) 2015-2019, Apple Inc.
 All rights reserved.
 */

#import <Foundation/NSObject.h>

NS_ASSUME_NONNULL_BEGIN

#pragma mark Unit Converters
/*
 NSUnitConverter describes how to convert a unit to and from the base unit of its dimension.  Subclass NSUnitConverter to implement new ways of converting a unit.
 */
API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitConverter : NSObject

/*
 The following methods perform conversions to and from the base unit of a unit class's dimension.  Each unit is defined against the base unit for the dimension to which the unit belongs.

 These methods are implemented differently depending on the type of conversion.  The default implementation in NSUnitConverter simply returns the value.

 These methods exist for the sole purpose of creating custom conversions for units in order to support converting a value from one kind of unit to another in the same dimension.  NSUnitConverter is an abstract class that is meant to be subclassed.  There is no need to call these methods directly to do a conversion -- the correct way to convert a measurement is to use [NSMeasurement measurementByConvertingToUnit:].  measurementByConvertingToUnit: uses the following 2 methods internally to perform the conversion.
 
 When creating a custom unit converter, you must override these two methods to implement the conversion to and from a value in terms of a unit and the corresponding value in terms of the base unit of that unit's dimension in order for conversion to work correctly.
 */

/*
 This method takes a value in terms of a unit and returns the corresponding value in terms of the base unit of the original unit's dimension.
 @param value Value in terms of the unit class
 @return Value in terms of the base unit
 */
- (double)baseUnitValueFromValue:(double)value;

/*
 This method takes in a value in terms of the base unit of a unit's dimension and returns the equivalent value in terms of the unit.
 @param baseUnitValue Value in terms of the base unit
 @return Value in terms of the unit class
 */
- (double)valueFromBaseUnitValue:(double)baseUnitValue;

@end


API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitConverterLinear : NSUnitConverter <NSSecureCoding> {
@private
    double _coefficient;
    double _constant;
}

/*
 For units that require linear conversion, the methods perform calculations in the form of y = ax + b, where
    - x is the value in terms of the unit on which this method is called
    - y is the value in terms of the base unit of the dimension
    - a is the known coefficient used for this unit's conversion
    - b is the known constant used for this unit's conversion

 baseUnitValueFromValue: performs the conversion in the form of y = ax + b, where x represents the value passed in and y represents the value returned.
 valueFromBaseUnitValue: performs the inverse conversion in the form of x = (y + (-1 * b))/a, where y represents the value passed in and x represents the value returned.

 An example of this is NSUnitTemperature.  For Celsius, baseUnitValueFromValue: calculates the value in Kelvin using the formula
            K = 1 * °C + 273.15
 and valueFromBaseUnitValue: calculates the value in Celsius using the formula
            C° = (K + (-1 * 273.15))/1
 where the coefficient is 1 and the constant is 273.15.

 For units that only require conversion by scale factor, the coefficient is the scale factor and the constant is always 0.  baseUnitValueFromValue: calculates the value in meters using the formula
            valueInMeters = 1000 * valueInKilometers + 0
 and valueFromBaseUnitValue: calculates the value in kilometers using the formula
            valueInKilometers = valueInMeters / 1000
 where the coefficient is 1000 and the constant is 0.  This API provides a convenience initializer initWithCoefficient: that assumes the constant is 0.
 */
@property (readonly) double coefficient;
@property (readonly) double constant;

- (instancetype)initWithCoefficient:(double)coefficient;

- (instancetype)initWithCoefficient:(double)coefficient constant:(double)constant NS_DESIGNATED_INITIALIZER;

@end


#pragma mark Unit
/*
 NSUnit is the base class for all unit types (dimensional and dimensionless).
 */
API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnit : NSObject <NSCopying, NSSecureCoding> {
@private
    NSString *_symbol;
}

@property (readonly, copy) NSString *symbol;

- (instancetype)init API_UNAVAILABLE(macos, ios, watchos, tvos);
+ (instancetype)new API_UNAVAILABLE(macos, ios, watchos, tvos);

- (instancetype)initWithSymbol:(NSString *)symbol NS_DESIGNATED_INITIALIZER;

@end


#pragma mark Dimensions

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSDimension : NSUnit <NSSecureCoding> {
@private
    NSUInteger _reserved;
@protected
    NSUnitConverter *_converter;
}

@property (readonly, copy) NSUnitConverter *converter;

- (instancetype)initWithSymbol:(NSString *)symbol converter:(NSUnitConverter *)converter NS_DESIGNATED_INITIALIZER;

/*
 This class method returns an instance of the dimension class that represents the base unit of that dimension.
 e.g.
    NSUnitSpeed *metersPerSecond = [NSUnitSpeed baseUnit];
 */
+ (instancetype)baseUnit;

@end


#pragma mark - Predefined Dimensions

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitAcceleration : NSDimension <NSSecureCoding>
/*
 Base unit - metersPerSecondSquared
 */

@property (class, readonly, copy) NSUnitAcceleration *metersPerSecondSquared;
@property (class, readonly, copy) NSUnitAcceleration *gravity;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitAngle : NSDimension <NSSecureCoding>
/*
 Base unit - degrees
 */

@property (class, readonly, copy) NSUnitAngle *degrees;
@property (class, readonly, copy) NSUnitAngle *arcMinutes;
@property (class, readonly, copy) NSUnitAngle *arcSeconds;
@property (class, readonly, copy) NSUnitAngle *radians;
@property (class, readonly, copy) NSUnitAngle *gradians;
@property (class, readonly, copy) NSUnitAngle *revolutions;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitArea : NSDimension <NSSecureCoding>
/*
 Base unit - squareMeters
 */

@property (class, readonly, copy) NSUnitArea *squareMegameters;
@property (class, readonly, copy) NSUnitArea *squareKilometers;
@property (class, readonly, copy) NSUnitArea *squareMeters;
@property (class, readonly, copy) NSUnitArea *squareCentimeters;
@property (class, readonly, copy) NSUnitArea *squareMillimeters;
@property (class, readonly, copy) NSUnitArea *squareMicrometers;
@property (class, readonly, copy) NSUnitArea *squareNanometers;
@property (class, readonly, copy) NSUnitArea *squareInches;
@property (class, readonly, copy) NSUnitArea *squareFeet;
@property (class, readonly, copy) NSUnitArea *squareYards;
@property (class, readonly, copy) NSUnitArea *squareMiles;
@property (class, readonly, copy) NSUnitArea *acres;
@property (class, readonly, copy) NSUnitArea *ares;
@property (class, readonly, copy) NSUnitArea *hectares;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitConcentrationMass : NSDimension <NSSecureCoding>
/*
 Base unit - gramsPerLiter
 */

@property (class, readonly, copy) NSUnitConcentrationMass *gramsPerLiter;
@property (class, readonly, copy) NSUnitConcentrationMass *milligramsPerDeciliter;

+ (NSUnitConcentrationMass *)millimolesPerLiterWithGramsPerMole:(double)gramsPerMole;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitDispersion : NSDimension <NSSecureCoding>
/*
 Base unit - partsPerMillion
 */
@property (class, readonly, copy) NSUnitDispersion *partsPerMillion;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitDuration : NSDimension <NSSecureCoding>  // Note: This class is not meant to be used for date calculation.  Use NSDate/NSCalendar/NSDateComponents for calendrical date and time operations.
/*
 Base unit - seconds
 */

@property (class, readonly, copy) NSUnitDuration *hours;
@property (class, readonly, copy) NSUnitDuration *minutes;
@property (class, readonly, copy) NSUnitDuration *seconds;
@property (class, readonly, copy) NSUnitDuration *milliseconds API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0), watchos(6.0));
@property (class, readonly, copy) NSUnitDuration *microseconds API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0), watchos(6.0));
@property (class, readonly, copy) NSUnitDuration *nanoseconds API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0), watchos(6.0));
@property (class, readonly, copy) NSUnitDuration *picoseconds API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0), watchos(6.0));

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitElectricCharge : NSDimension <NSSecureCoding>
/*
 Base unit - coulombs
 */

@property (class, readonly, copy) NSUnitElectricCharge *coulombs;
@property (class, readonly, copy) NSUnitElectricCharge *megaampereHours;
@property (class, readonly, copy) NSUnitElectricCharge *kiloampereHours;
@property (class, readonly, copy) NSUnitElectricCharge *ampereHours;
@property (class, readonly, copy) NSUnitElectricCharge *milliampereHours;
@property (class, readonly, copy) NSUnitElectricCharge *microampereHours;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitElectricCurrent : NSDimension <NSSecureCoding>
/*
 Base unit - amperes
 */

@property (class, readonly, copy) NSUnitElectricCurrent *megaamperes;
@property (class, readonly, copy) NSUnitElectricCurrent *kiloamperes;
@property (class, readonly, copy) NSUnitElectricCurrent *amperes;
@property (class, readonly, copy) NSUnitElectricCurrent *milliamperes;
@property (class, readonly, copy) NSUnitElectricCurrent *microamperes;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitElectricPotentialDifference : NSDimension <NSSecureCoding>
/*
 Base unit - volts
 */

@property (class, readonly, copy) NSUnitElectricPotentialDifference *megavolts;
@property (class, readonly, copy) NSUnitElectricPotentialDifference *kilovolts;
@property (class, readonly, copy) NSUnitElectricPotentialDifference *volts;
@property (class, readonly, copy) NSUnitElectricPotentialDifference *millivolts;
@property (class, readonly, copy) NSUnitElectricPotentialDifference *microvolts;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitElectricResistance : NSDimension <NSSecureCoding>
/*
 Base unit - ohms
 */

@property (class, readonly, copy) NSUnitElectricResistance *megaohms;
@property (class, readonly, copy) NSUnitElectricResistance *kiloohms;
@property (class, readonly, copy) NSUnitElectricResistance *ohms;
@property (class, readonly, copy) NSUnitElectricResistance *milliohms;
@property (class, readonly, copy) NSUnitElectricResistance *microohms;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitEnergy : NSDimension <NSSecureCoding>
/*
 Base unit - joules
 */

@property (class, readonly, copy) NSUnitEnergy *kilojoules;
@property (class, readonly, copy) NSUnitEnergy *joules;
@property (class, readonly, copy) NSUnitEnergy *kilocalories;
@property (class, readonly, copy) NSUnitEnergy *calories;
@property (class, readonly, copy) NSUnitEnergy *kilowattHours;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitFrequency : NSDimension <NSSecureCoding>
/*
 Base unit - hertz
 */

@property (class, readonly, copy) NSUnitFrequency *terahertz;
@property (class, readonly, copy) NSUnitFrequency *gigahertz;
@property (class, readonly, copy) NSUnitFrequency *megahertz;
@property (class, readonly, copy) NSUnitFrequency *kilohertz;
@property (class, readonly, copy) NSUnitFrequency *hertz;
@property (class, readonly, copy) NSUnitFrequency *millihertz;
@property (class, readonly, copy) NSUnitFrequency *microhertz;
@property (class, readonly, copy) NSUnitFrequency *nanohertz;

// 1 FPS ≡ 1 Hertz
@property (class, readonly, copy) NSUnitFrequency *framesPerSecond API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0), watchos(6.0));

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitFuelEfficiency : NSDimension <NSSecureCoding>
/*
 Base unit - litersPer100Kilometers
 */

@property (class, readonly, copy) NSUnitFuelEfficiency *litersPer100Kilometers;
@property (class, readonly, copy) NSUnitFuelEfficiency *milesPerImperialGallon;
@property (class, readonly, copy) NSUnitFuelEfficiency *milesPerGallon;

@end

/*
 A dimension for representing amounts of digital information.
 Base Unit: Byte

 The values of the below follow IEC 80000-13 definitions and conventions.
 */
API_AVAILABLE(macos(10.15), ios(13.0), tvos(13.0), watchos(6.0))
NS_SWIFT_NAME(UnitInformationStorage)
@interface NSUnitInformationStorage : NSDimension <NSSecureCoding>

// Bytes are defined by IEC 80000-13: one byte is 8 bits.
@property (readonly, class, copy) NSUnitInformationStorage *bytes;

// One byte is 8 bits; one nibble is 4 bits.
@property (readonly, class, copy) NSUnitInformationStorage *bits;
@property (readonly, class, copy) NSUnitInformationStorage *nibbles;

// SI-prefixed units (i.e. base 10):
// 1 kilobyte = 1000¹ bytes; 1 megabyte = 1000² bytes; etc.
@property (readonly, class, copy) NSUnitInformationStorage *yottabytes;
@property (readonly, class, copy) NSUnitInformationStorage *zettabytes;
@property (readonly, class, copy) NSUnitInformationStorage *exabytes;
@property (readonly, class, copy) NSUnitInformationStorage *petabytes;
@property (readonly, class, copy) NSUnitInformationStorage *terabytes;
@property (readonly, class, copy) NSUnitInformationStorage *gigabytes;
@property (readonly, class, copy) NSUnitInformationStorage *megabytes;
@property (readonly, class, copy) NSUnitInformationStorage *kilobytes;

@property (readonly, class, copy) NSUnitInformationStorage *yottabits;
@property (readonly, class, copy) NSUnitInformationStorage *zettabits;
@property (readonly, class, copy) NSUnitInformationStorage *exabits;
@property (readonly, class, copy) NSUnitInformationStorage *petabits;
@property (readonly, class, copy) NSUnitInformationStorage *terabits;
@property (readonly, class, copy) NSUnitInformationStorage *gigabits;
@property (readonly, class, copy) NSUnitInformationStorage *megabits;
@property (readonly, class, copy) NSUnitInformationStorage *kilobits;

// IEC-prefixed units (i.e. base 2):
// 1 kibibyte = 1024¹ bytes; 1 mebibyte = 1024² bytes; etc.
@property (readonly, class, copy) NSUnitInformationStorage *yobibytes;
@property (readonly, class, copy) NSUnitInformationStorage *zebibytes;
@property (readonly, class, copy) NSUnitInformationStorage *exbibytes;
@property (readonly, class, copy) NSUnitInformationStorage *pebibytes;
@property (readonly, class, copy) NSUnitInformationStorage *tebibytes;
@property (readonly, class, copy) NSUnitInformationStorage *gibibytes;
@property (readonly, class, copy) NSUnitInformationStorage *mebibytes;
@property (readonly, class, copy) NSUnitInformationStorage *kibibytes;

@property (readonly, class, copy) NSUnitInformationStorage *yobibits;
@property (readonly, class, copy) NSUnitInformationStorage *zebibits;
@property (readonly, class, copy) NSUnitInformationStorage *exbibits;
@property (readonly, class, copy) NSUnitInformationStorage *pebibits;
@property (readonly, class, copy) NSUnitInformationStorage *tebibits;
@property (readonly, class, copy) NSUnitInformationStorage *gibibits;
@property (readonly, class, copy) NSUnitInformationStorage *mebibits;
@property (readonly, class, copy) NSUnitInformationStorage *kibibits;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitLength : NSDimension <NSSecureCoding>
/*
 Base unit - meters
 */

@property (class, readonly, copy) NSUnitLength *megameters;
@property (class, readonly, copy) NSUnitLength *kilometers;
@property (class, readonly, copy) NSUnitLength *hectometers;
@property (class, readonly, copy) NSUnitLength *decameters;
@property (class, readonly, copy) NSUnitLength *meters;
@property (class, readonly, copy) NSUnitLength *decimeters;
@property (class, readonly, copy) NSUnitLength *centimeters;
@property (class, readonly, copy) NSUnitLength *millimeters;
@property (class, readonly, copy) NSUnitLength *micrometers;
@property (class, readonly, copy) NSUnitLength *nanometers;
@property (class, readonly, copy) NSUnitLength *picometers;
@property (class, readonly, copy) NSUnitLength *inches;
@property (class, readonly, copy) NSUnitLength *feet;
@property (class, readonly, copy) NSUnitLength *yards;
@property (class, readonly, copy) NSUnitLength *miles;
@property (class, readonly, copy) NSUnitLength *scandinavianMiles;
@property (class, readonly, copy) NSUnitLength *lightyears;
@property (class, readonly, copy) NSUnitLength *nauticalMiles;
@property (class, readonly, copy) NSUnitLength *fathoms;
@property (class, readonly, copy) NSUnitLength *furlongs;
@property (class, readonly, copy) NSUnitLength *astronomicalUnits;
@property (class, readonly, copy) NSUnitLength *parsecs;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitIlluminance : NSDimension <NSSecureCoding>
/*
 Base unit - lux
 */

@property (class, readonly, copy) NSUnitIlluminance *lux;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitMass : NSDimension <NSSecureCoding>
/*
 Base unit - kilograms
 */

@property (class, readonly, copy) NSUnitMass *kilograms;
@property (class, readonly, copy) NSUnitMass *grams;
@property (class, readonly, copy) NSUnitMass *decigrams;
@property (class, readonly, copy) NSUnitMass *centigrams;
@property (class, readonly, copy) NSUnitMass *milligrams;
@property (class, readonly, copy) NSUnitMass *micrograms;
@property (class, readonly, copy) NSUnitMass *nanograms;
@property (class, readonly, copy) NSUnitMass *picograms;
@property (class, readonly, copy) NSUnitMass *ounces;
@property (class, readonly, copy) NSUnitMass *poundsMass;
@property (class, readonly, copy) NSUnitMass *stones;
@property (class, readonly, copy) NSUnitMass *metricTons;
@property (class, readonly, copy) NSUnitMass *shortTons;
@property (class, readonly, copy) NSUnitMass *carats;
@property (class, readonly, copy) NSUnitMass *ouncesTroy;
@property (class, readonly, copy) NSUnitMass *slugs;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitPower : NSDimension <NSSecureCoding>
/*
 Base unit - watts
 */

@property (class, readonly, copy) NSUnitPower *terawatts;
@property (class, readonly, copy) NSUnitPower *gigawatts;
@property (class, readonly, copy) NSUnitPower *megawatts;
@property (class, readonly, copy) NSUnitPower *kilowatts;
@property (class, readonly, copy) NSUnitPower *watts;
@property (class, readonly, copy) NSUnitPower *milliwatts;
@property (class, readonly, copy) NSUnitPower *microwatts;
@property (class, readonly, copy) NSUnitPower *nanowatts;
@property (class, readonly, copy) NSUnitPower *picowatts;
@property (class, readonly, copy) NSUnitPower *femtowatts;
@property (class, readonly, copy) NSUnitPower *horsepower;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitPressure : NSDimension <NSSecureCoding>
/*
 Base unit - newtonsPerMetersSquared (equivalent to 1 pascal)
 */

@property (class, readonly, copy) NSUnitPressure *newtonsPerMetersSquared;
@property (class, readonly, copy) NSUnitPressure *gigapascals;
@property (class, readonly, copy) NSUnitPressure *megapascals;
@property (class, readonly, copy) NSUnitPressure *kilopascals;
@property (class, readonly, copy) NSUnitPressure *hectopascals;
@property (class, readonly, copy) NSUnitPressure *inchesOfMercury;
@property (class, readonly, copy) NSUnitPressure *bars;
@property (class, readonly, copy) NSUnitPressure *millibars;
@property (class, readonly, copy) NSUnitPressure *millimetersOfMercury;
@property (class, readonly, copy) NSUnitPressure *poundsForcePerSquareInch;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitSpeed : NSDimension <NSSecureCoding>
/*
 Base unit - metersPerSecond
 */

@property (class, readonly, copy) NSUnitSpeed *metersPerSecond;
@property (class, readonly, copy) NSUnitSpeed *kilometersPerHour;
@property (class, readonly, copy) NSUnitSpeed *milesPerHour;
@property (class, readonly, copy) NSUnitSpeed *knots;

@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitTemperature : NSDimension <NSSecureCoding>
/*
 Base unit - kelvin
 */
@property (class, readonly, copy) NSUnitTemperature *kelvin;
@property (class, readonly, copy) NSUnitTemperature *celsius; 
@property (class, readonly, copy) NSUnitTemperature *fahrenheit;


@end

API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSUnitVolume : NSDimension <NSSecureCoding>
/*
 Base unit - liters
 */

@property (class, readonly, copy) NSUnitVolume *megaliters;
@property (class, readonly, copy) NSUnitVolume *kiloliters;
@property (class, readonly, copy) NSUnitVolume *liters;
@property (class, readonly, copy) NSUnitVolume *deciliters;
@property (class, readonly, copy) NSUnitVolume *centiliters;
@property (class, readonly, copy) NSUnitVolume *milliliters;
@property (class, readonly, copy) NSUnitVolume *cubicKilometers;
@property (class, readonly, copy) NSUnitVolume *cubicMeters;
@property (class, readonly, copy) NSUnitVolume *cubicDecimeters;
@property (class, readonly, copy) NSUnitVolume *cubicCentimeters;
@property (class, readonly, copy) NSUnitVolume *cubicMillimeters;
@property (class, readonly, copy) NSUnitVolume *cubicInches;
@property (class, readonly, copy) NSUnitVolume *cubicFeet;
@property (class, readonly, copy) NSUnitVolume *cubicYards;
@property (class, readonly, copy) NSUnitVolume *cubicMiles;
@property (class, readonly, copy) NSUnitVolume *acreFeet;
@property (class, readonly, copy) NSUnitVolume *bushels;
@property (class, readonly, copy) NSUnitVolume *teaspoons;
@property (class, readonly, copy) NSUnitVolume *tablespoons;
@property (class, readonly, copy) NSUnitVolume *fluidOunces;
@property (class, readonly, copy) NSUnitVolume *cups;
@property (class, readonly, copy) NSUnitVolume *pints;
@property (class, readonly, copy) NSUnitVolume *quarts;
@property (class, readonly, copy) NSUnitVolume *gallons;
@property (class, readonly, copy) NSUnitVolume *imperialTeaspoons;
@property (class, readonly, copy) NSUnitVolume *imperialTablespoons;
@property (class, readonly, copy) NSUnitVolume *imperialFluidOunces;
@property (class, readonly, copy) NSUnitVolume *imperialPints;
@property (class, readonly, copy) NSUnitVolume *imperialQuarts;
@property (class, readonly, copy) NSUnitVolume *imperialGallons;
@property (class, readonly, copy) NSUnitVolume *metricCups;

@end

NS_ASSUME_NONNULL_END
