#ifndef RecoCTPPS_ProtonProducer_FillNumberLUTHandler_h
#define RecoCTPPS_ProtonProducer_FillNumberLUTHandler_h

#include <fstream>

#include <iostream>

namespace CTPPSAlCa
{
    class FillNumberLUTHandler
    {
        public:
            inline FillNumberLUTHandler( const char* file ):
                valid_( false )
            {
                std::ifstream f( file );
                if ( !f.is_open() ) return;
                std::string ss;
                while ( f >> ss ) {
                    size_t pos = ss.find( ":" );
                    if ( pos==std::string::npos ) continue;
                    unsigned int run  = atoi( ss.substr( 0, pos ).c_str() ),
                                 fill = atoi( ss.substr( pos+1, ss.size() ).c_str() );
                    fillInRun_[run] = fill;
                }
                valid_ = true;
            }

            inline unsigned int getFillNumber( unsigned int run ) const {
                if ( !valid_ or !fillInRun_.count( run ) ) return 0;
                return fillInRun_.at( run );
            }

            inline bool isValid() const { return valid_; }

        private:
            bool valid_;
            std::map<unsigned int,unsigned int> fillInRun_;
    };
}

#endif
