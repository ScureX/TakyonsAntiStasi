class TKY_SaveManager<Class T>
{
    protected string m_sFilePath;
    protected FileHandle m_LockFile;
	protected const float LOCK_RETRY_INTERVAL = 0.1;  // Time in seconds between lock retries
    protected const float LOCK_TIMEOUT = 5.0;        // Max time to wait before giving up

    // Constructor to set the file path
    void TKY_SaveManager(string filePath)
    {
        m_sFilePath = filePath;
    }
	
	// Method to acquire a file lock with a timeout and retry mechanism
    protected bool WaitForLock(float timeout)
    {
        float elapsedTime = 0.0;

        while (elapsedTime < timeout)
        {
            // Try to acquire the lock file
            if (LockFile())
            {
                return true;  // Successfully acquired lock
            }

            // If lock isn't acquired, wait for a short interval
            Sleep(LOCK_RETRY_INTERVAL);
            elapsedTime += LOCK_RETRY_INTERVAL;
        }

        // Failed to acquire lock within timeout
        return false;
    }

    // Method to save generic data to a JSON file
    bool SaveData(notnull ref T data)
    {
        // Attempt to acquire a file lock with timeout and retry
        if (!WaitForLock(LOCK_TIMEOUT))
        {
            Print("Failed to acquire lock for " + m_sFilePath + " within timeout.");
            return false;
        }

        // Attempt to save the generic data to a JSON file
        bool success = JsonFileLoader<T>.JsonSaveFile(m_sFilePath, data);
        if (success)
        {
            Print("Data successfully saved to " + m_sFilePath);
        }
        else
        {
            Print("Failed to save data to " + m_sFilePath);
        }

        // Release the lock after writing
        UnlockFile();
        return success;
    }

    // Method to load generic data from a JSON file
    bool LoadData(out T data)
    {
        // Initialize the data object
        data = new T();

        // Attempt to load the generic data from a JSON file
        bool success = JsonFileLoader<T>.JsonLoadFile(m_sFilePath, data);
        if (success)
        {
            Print("Data successfully loaded from " + m_sFilePath);
        }
        else
        {
            Print("Failed to load data from " + m_sFilePath);
        }

        return success;
    }

    // Method to clear the data in the JSON file (write an empty object)
    bool ClearData()
    {
        T emptyData = new T();
        return SaveData(emptyData);
    }

    // Acquire a file lock to prevent simultaneous write operations
    protected bool LockFile()
    {
        // Try to open a lock file in write mode
        m_LockFile = OpenFile(m_sFilePath + ".lock", FileMode.WRITE);
        if (m_LockFile == null)
        {
            Print("Failed to create lock file for " + m_sFilePath);
            return false;
        }

        // Return true if lock file is successfully created
        return true;
    }

    // Release the file lock after the write operation is done
    protected void UnlockFile()
    {
        if (m_LockFile != null)
        {
            CloseFile(m_LockFile);
            m_LockFile = null;

            // Optionally, delete the lock file
            DeleteFile(m_sFilePath + ".lock");
        }
    }
}
