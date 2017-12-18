/***************************************************************
 * Name:      ewxDynArray.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2011-01-15
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef EWXDYNARRAY_H
#define EWXDYNARRAY_H

#include <vector>

template <typename T> class ewxDyn2DArray
{
    public:
        ewxDyn2DArray():m_data(0, std::vector<T>(0)){m_cols = 0;m_rows = 0;};
        ewxDyn2DArray(int cols, int rows):m_data(cols, std::vector<T>(rows)){m_cols=cols;m_rows=rows;};
        ewxDyn2DArray(const ewxDyn2DArray& other)
        {
            resize(other.m_cols, other.m_rows);
            for (int i = 0; i < m_cols; i++)
            m_data[i] = other.m_data[i];
        };
        inline std::vector<T> & operator[](int i) { return m_data[i];}
        inline const std::vector<T> & operator[] (int i) const { return m_data[i];}
        ewxDyn2DArray& operator=(const ewxDyn2DArray& other)
        {
            if (this == &other) return *this; // handle self assignment
            resize(other.m_cols, other.m_rows);
            for (int i = 0; i < m_cols; i++)
            m_data[i] = other.m_data[i];
            return *this;
        };

        void resize(int cols, int rows)
        {
            m_data.resize(cols);
            for(int i = 0;i < cols;++i) m_data[i].resize(rows);
            m_cols = cols;
            m_rows = rows;
        }
    private:
        int m_cols;
        int m_rows;
        std::vector<std::vector<T> > m_data;
};

template <typename T> class ewxDynArray
{
    public:
        ewxDynArray():m_data(0){m_items = 0;};
        ewxDynArray(int items):m_data(items){m_items=items;};
        ewxDynArray(const ewxDynArray& other)
        {
            for (int i = 0; i < m_items; i++)
            m_data[i] = other.m_data[i];
        };
        inline T & operator[](int i) { return m_data[i];}
        inline const T & operator[] (int i) const { return m_data[i];}
        ewxDynArray& operator=(const ewxDynArray& other)
        {
            if (this == &other) return *this; // handle self assignment
            for (int i = 0; i < m_items; i++)
            m_data[i] = other.m_data[i];
            return *this;
        };

        void resize(int items)
        {
            m_data.resize(items);
            m_items = items;
        }
    private:
        int m_items;
        std::vector<T> m_data;
};


#endif // EWXDYNARRAY_H
