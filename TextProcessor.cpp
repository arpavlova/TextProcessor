//#include "CommandManager.hpp"
//
//int main()
//{
//    try
//    {
//        TheCommandManager::getInstance().introduction();
//        while (TheCommandManager::getInstance().isRunning())
//        {
//            TheCommandManager::getInstance().handleInput();
//            TheCommandManager::getInstance().execute();
//        }
//    }
//    catch (...)
//    {
//        std::cout << "Something bad happened!:(\n";
//    }
//    return 0;
//}
////alias agi open agi.txt;addline 1 agi bagi;addline 1 line 1;saveas agi.txt;exit