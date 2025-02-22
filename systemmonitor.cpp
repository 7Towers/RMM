#include "systemmonitor.h"
#include <QProcess>
#include <QDir>

SystemMonitor::SystemMonitor()
{
}

SystemMonitor::~SystemMonitor()
{
    cleanupProcessRefs();
}

QQmlListProperty<ProcessMetrics> SystemMonitor::processes()
{
    return {this, &m_processes};
}

void SystemMonitor::cleanupProcessRefs()
{
    for (auto& process : m_processes) {
        delete process;
    }
    m_processes.clear();
}

void SystemMonitor::updateProcessList()
{
    cleanupProcessRefs();
    auto processList = getProcessList();
    for (auto& process : processList) {
        auto pm = new ProcessMetrics();
        pm->setProcessName(QString::fromStdString(process.second));
        pm->setPid(process.first);
        qDebug() << "Process: " << process.second.c_str();
        // pm->setCPUUsage(getCPUUsage(process.first));
        // pm->setCPUPercentage(getCPUPercentage(process.first));
        this->m_processes.append(pm);
    }

    emit processesChanged();
}

double SystemMonitor::getCPUPercentage(pid_t pid)
{
        double usage1 = getCPUUsage(pid);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        double usage2 = getCPUUsage(pid);

        return (usage2 - usage1) * 100.0;
}

double SystemMonitor::getCPUUsage(pid_t pid)
{
    QProcess process;
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)  // or OSX
        // pid_t pid = getpid();
        struct proc_taskinfo pti;
        int result = proc_pidinfo(pid, PROC_PIDTASKINFO, 0, &pti, sizeof(pti));
        if (result <= 0) return -1.0;

        return (pti.pti_total_user + pti.pti_total_system) / (double)sysconf(_SC_CLK_TCK);
        // process.start("top -bn1");
#elif defined(Q_OS_WIN)
        process.start("wmic cpu get loadpercentage");
#endif
    // process.waitForFinished();
    // QString output = process.readAllStandardOutput();
    // qDebug() << output;
    // Parse the output to extract CPU usage
    // Return the parsed value
    // return output;
    return 0;
}

std::vector<std::pair<pid_t, std::string>> SystemMonitor::getProcessList() {
    std::vector<std::pair<pid_t, std::string>> processList;
    pid_t pids[2048];
    int bytes = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    int nPids = bytes / sizeof(pid_t);

    for (int i = 0; i < nPids; i++) {
        pid_t pid = pids[i];
        if (pid == 0) continue;

        char name[PROC_PIDPATHINFO_MAXSIZE];
        if (proc_name(pid, name, sizeof(name)) <= 0) continue;

        processList.emplace_back(pid, std::string(name));
    }

    return processList;
}

// void SystemMonitor::processUpdated(ProcessMetrics pm)
// {
//     for (auto& process : m_processes) {
//         if (process->processName() == pm.processName()) {
//             process->setCPUUsage(pm.cpuUsage());
//             process->setCPUPercentage(pm.cpuPercentage());
//             process->setRAMUsage(pm.RAMUsage());
//             process->setRAMPercentage(pm.RAMPercentage());
//             break;
//         }
//     }
// }
