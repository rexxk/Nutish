#ifndef __NUTLIB_TIMESTEP_H
#define __NUTLIB_TIMESTEP_H


namespace Nut
{


	class Timestep
	{
	public:
		Timestep()
			: m_Timestep(0.0)  //, m_LastTime(0.0)
		{

		}

		void Update(double time)
		{
//			m_Timestep = time - m_LastTime;
//			m_LastTime = time;

			m_Timestep = time;
		}

		operator double()
		{
			return m_Timestep;
		}

	private:
		double m_Timestep;
//		double m_LastTime;
	};


}


#endif
