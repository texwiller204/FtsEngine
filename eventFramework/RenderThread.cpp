#include "stdafx.h"
#include "RenderThread.h"

#include "GameObject.h"
#include "XFormBuffer.h"

//https://github.com/nyaxt/ptnk/blob/eab15a153f6b6077d1b1309a78d0f21ad9bdc39a/ptnk/helperthr.cpp


namespace  BubbleBattleBoids
{

	RenderThread::RenderThread(): m_isCancelled(false)
	{
		m_thr.reset(new std::thread([this]() { thrmain(); }));
	}

	RenderThread::~RenderThread()
	{
		m_isCancelled = true;
		m_condJobArrival.notify_all();

		m_thr->join();
	}

	void RenderThread::Send(Job job)
	{
		std::lock_guard<std::mutex> g(m_mtxJobs);

		m_jobq.push(job);
		m_condJobArrival.notify_all();
	}


	void RenderThread::thrmain()
	{
			while (!m_isCancelled)
			{
				Job job;

				// wait next job
				{
					std::unique_lock<std::mutex> g(m_mtxJobs);

					while (m_jobq.empty())
					{
						// jobq empty...

						// wait cond for new job arrival
						m_condJobArrival.wait_for(g, std::chrono::milliseconds(10));

						if (m_isCancelled) 
							return;
					}


					//std::swap(job, m_jobq.front());
					//m_jobq.pop();
				}

				if (m_isCancelled) 
					return;

				job();

			}


			while (!m_isCancelled)
			{

				// Read from other buffer
				if (XFormBufferManager::GetInstance().ReadBufferDirty())
				{
					XFormBufferManager::GetInstance().SwapReadBuffers();

					CleanUp();
				}

			}

		}

	void RenderThread::CleanUp()
	{
		// Destroy dead objects
		while (!mDeadObjects.empty())
		{
			delete mDeadObjects.back();
			mDeadObjects.pop_back();
		}
	}
} // end of namespace ptnk