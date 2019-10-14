#ifndef TYPETRAITS_H
#define TYPETRAITS_H

#include <type_traits>

#include <QList>
#include <QVector>
#include <QMap>
#include <QHash>

template<typename T> struct is_qt_array_type : std::false_type{};
template<typename T> struct is_qt_array_type<QList<T>> : std::true_type{};
template<typename T> struct is_qt_array_type<QVector<T>> : std::true_type{};

template<typename V> struct is_qt_dictionary_type : std::false_type{};
template<typename K, typename V> struct is_qt_dictionary_type<QMap<K, V>> : std::true_type{};
template<typename K, typename V> struct is_qt_dictionary_type<QHash<K, V>> : std::true_type{};

#endif // TYPETRAITS_H
