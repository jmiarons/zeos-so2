#include <perrormessage.h>

char* errno_output(int num) {
switch (num) {
        case 1:
                return "Operation not permitted";
                break;
        case 2:
                return "No such file or directory";
                break;
        case 3:
                return "No such process";
                break;
        case 4:
                return "Interrupted system call";
                break;
        case 5:
                return "Input/output error";
                break;
        case 6:
                return "No such device or address";
                break;
        case 7:
                return "Argument list too long";
                break;
        case 8:
                return "Exec format error";
                break;
        case 9:
                return "Bad file descriptor";
                break;
        case 10:
                return "No child processes";
                break;
        case 11:
                return "Resource temporarily unavailable";
                break;
        case 12:
                return "Cannot allocate memory";
                break;
        case 13:
                return "Permission denied";
                break;
        case 14:
                return "Bad address";
                break;
        case 15:
                return "Block device required";
                break;
        case 16:
                return "Device or resource busy";
                break;
        case 17:
                return "File exists";
                break;
        case 18:
                return "Invalid cross-device link";
                break;
        case 19:
                return "No such device";
                break;
        case 20:
                return "Not a directory";
                break;
        case 21:
                return "Is a directory";
                break;
        case 22:
                return "Invalid argument";
                break;
        case 23:
                return "Too many open files in system";
                break;
        case 24:
                return "Too many open files";
                break;
        case 25:
                return "Inappropriate ioctl for device";
                break;
        case 26:
                return "Text file busy";
                break;
        case 27:
                return "File too large";
                break;
        case 28:
                return "No space left on device";
                break;
        case 29:
                return "Illegal seek";
                break;
        case 30:
                return "Read-only file system";
                break;
        case 31:
                return "Too many links";
                break;
        case 32:
                return "Broken pipe";
                break;
        case 33:
                return "Numerical argument out of domain";
                break;
        case 34:
                return "Numerical result out of range";
                break;
        case 35:
                return "Resource deadlock avoided";
                break;
        case 36:
                return "File name too long";
                break;
        case 37:
                return "No locks available";
                break;
        case 38:
                return "Function not implemented";
                break;
        case 39:
                return "Directory not empty";
                break;
        case 40:
                return "Too many levels of symbolic links";
                break;
        case 41:
                return "Resource temporarily unavailable";
                break;
        case 42:
                return "No message of desired type";
                break;
        case 43:
                return "Identifier removed";
                break;
        case 44:
                return "Channel number out of range";
                break;
        case 45:
                return "Level 2 not synchronized";
                break;
        case 46:
                return "Level 3 halted";
                break;
        case 47:
                return "Level 3 reset";
                break;
        case 48:
                return "Link number out of range";
                break;
        case 49:
                return "Protocol driver not attached";
                break;
        case 50:
                return "No CSI structure available";
                break;
        case 51:
                return "Level 2 halted";
                break;
        case 52:
                return "Invalid exchange";
                break;
        case 53:
                return "Invalid request descriptor";
                break;
        case 54:
                return "Exchange full";
                break;
        case 55:
                return "No anode";
                break;
        case 56:
                return "Invalid request code";
                break;
        case 57:
                return "Invalid slot";
                break;
        case 58:
                return "Resource deadlock avoided";
                break;
        case 59:
                return "Bad font file format";
                break;
        case 60:
                return "Device not a stream";
                break;
        case 61:
                return "No data available";
                break;
        case 62:
                return "Timer expired";
                break;
        case 63:
                return "Out of streams resources";
                break;
        case 64:
                return "Machine is not on the network";
                break;
        case 65:
                return "Package not installed";
                break;
        case 66:
                return "Object is remote";
                break;
        case 67:
                return "Link has been severed";
                break;
        case 68:
                return "Advertise error";
                break;
        case 69:
                return "Srmount error";
                break;
        case 70:
                return "Communication error on send";
                break;
        case 71:
                return "Protocol error";
                break;
        case 72:
                return "Multihop attempted";
                break;
        case 73:
                return "RFS specific error";
                break;
        case 74:
                return "Bad message";
                break;
        case 75:
                return "Value too large for defined data type";
                break;
        case 76:
                return "Name not unique on network";
                break;
        case 77:
                return "File descriptor in bad state";
                break;
        case 78:
                return "Remote address changed";
                break;
        case 79:
                return "Can not access a needed shared library";
                break;
        case 80:
                return "Accessing a corrupted shared library";
                break;
        case 81:
                return ".lib section in a.out corrupted";
                break;
        case 82:
                return "Attempting to link in too many shared libraries";
                break;
        case 83:
                return "Cannot exec a shared library directly";
                break;
        case 84:
                return "Invalid or incomplete multibyte or wide character";
                break;
        case 85:
                return "Interrupted system call should be restarted";
                break;
        case 86:
                return "Streams pipe error";
                break;
        case 87:
                return "Too many users";
                break;
        case 88:
                return "Socket operation on non-socket";
                break;
        case 89:
                return "Destination address required";
                break;
        case 90:
                return "Message too long";
                break;
        case 91:
                return "Protocol wrong type for socket";
                break;
        case 92:
                return "Protocol not available";
                break;
        case 93:
                return "Protocol not supported";
                break;
        case 94:
                return "Socket type not supported";
                break;
        case 95:
                return "Operation not supported";
                break;
        case 96:
                return "Protocol family not supported";
                break;
        case 97:
                return "Address family not supported by protocol";
                break;
        case 98:
                return "Address already in use";
                break;
        case 99:
                return "Cannot assign requested address";
                break;
        case 100:
                return "Network is down";
                break;
        case 101:
                return "Network is unreachable";
                break;
        case 102:
                return "Network dropped connection on reset";
                break;
        case 103:
                return "Software caused connection abort";
                break;
        case 104:
                return "Connection reset by peer";
                break;
        case 105:
                return "No buffer space available";
                break;
        case 106:
                return "Transport endpoint is already connected";
                break;
        case 107:
                return "Transport endpoint is not connected";
                break;
        case 108:
                return "Cannot send after transport endpoint shutdown";
                break;
        case 109:
                return "Too many references: cannot splice";
                break;
        case 110:
                return "Connection timed out";
                break;
        case 111:
                return "Connection refused";
                break;
        case 112:
                return "Host is down";
                break;
        case 113:
                return "No route to host";
                break;
        case 114:
                return "Operation already in progress";
                break;
        case 115:
                return "Operation now in progress";
                break;
        case 116:
                return "Stale file handle";
                break;
        case 117:
                return "Structure needs cleaning";
                break;
        case 118:
                return "Not a XENIX named type file";
                break;
        case 119:
                return "No XENIX semaphores available";
                break;
        case 120:
                return "Is a named type file";
                break;
        case 121:
                return "Remote I/O error";
                break;
        case 122:
                return "Disk quota exceeded";
                break;
        case 123:
                return "No medium found";
                break;
        case 124:
                return "Wrong medium type";
                break;
        case 125:
                return "Operation canceled";
                break;
        case 126:
                return "Required key not available";
                break;
        case 127:
                return "Key has expired";
                break;
        case 128:
                return "Key has been revoked";
                break;
        case 129:
                return "Key was rejected by service";
                break;
        case 130:
                return "Owner died";
                break;
        case 131:
                return "State not recoverable";
                break;
        case 132:
                return "Operation not possible due to RF-kill";
                break;
        case 133:
                return "Memory page has hardware error";
                break;
        default:
                return "No error\n";
                break;
}
}
