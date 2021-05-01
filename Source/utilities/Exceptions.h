#pragma once

#include <exception>
#include <string>

namespace inspector {
    struct MonitorBlockException : public std::exception {
        const char* id;
        std::string message {};
         

        MonitorBlockException(const char* id, const char* message) {
            this->id = id;
            this->message.append("Monitor block (id ");
            this->message.append(id);
            this->message.append("): ");
            this->message.append(message);
        }

        const char* what() const throw() {
            return message.c_str();
        }
    };

    struct WorkflowException : public std::exception {
        const char* id;
        std::string message {};
         

        WorkflowException(const char* id, const char* message) {
            this->id = id;
            this->message.append("Workflow (id ");
            this->message.append(id);
            this->message.append("): ");
            this->message.append(message);
        }

        const char* what() const throw() {
            return message.c_str();
        }
    };

    struct DispatcherException : public std::exception {
        const char* id;
        std::string message {};
         

        DispatcherException(const char* id, const char* message) {
            this->id = id;
            this->message.append("Dispatcher encountered an error. Details: ");
            this->message.append(message);
        }

        const char* what() const throw() {
            return message.c_str();
        }
    };

}