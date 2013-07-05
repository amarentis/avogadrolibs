/******************************************************************************

  This source file is part of the Avogadro project.

  Copyright 2013 Kitware, Inc.

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#ifndef AVOGADRO_IO_FILEFORMAT_H
#define AVOGADRO_IO_FILEFORMAT_H

#include "avogadroioexport.h"
#include <avogadro/core/avogadrocore.h>

#include <string>
#include <vector>
#include <istream>
#include <ostream>

namespace Avogadro {

namespace Core {
class Molecule;
}

namespace Io {

/**
 * @class FileFormat fileformat.h <avogadro/io/fileformat.h>
 * @brief General API for file formats.
 * @author Marcus D. Hanwell
 *
 * This serves as the common base class for chemical file formats. Classes
 * deriving from this one override the read and write virtual methods and
 * operate on the given streams. Several other signatures are available for
 * convenience. If there is an error reading or writing a file the string
 * returned by error() will give more details.
 */

class AVOGADROIO_EXPORT FileFormat
{
public:
  FileFormat();
  virtual ~FileFormat();

  /**
   * @brief Flags defining supported operations.
   */
  enum Operation {
    None      = 0x0,
    Read      = 0x1,
    Write     = 0x2,
    ReadWrite = Read | Write,

    Stream    = 0x10,
    String    = 0x20,
    File      = 0x40,

    All       = ReadWrite | Stream | String | File
  };
  typedef int Operations;

  /**
   * @return Operation flags defining the capabilities of this format.
   */
  virtual Operations supportedOperations() const = 0;

  /**
   * @brief Open the specified file in Read or Write mode.
   * @return True on success, false on failure.
   */
  bool open(const std::string &fileName, Operation mode);

  /**
   * @brief Close any opened file handles.
   */
  void close();

  /**
   * @brief Read in a molecule, if there are no molecules to read molecule will
   * be empty. This can be used to read in one or more molecules from a given
   * file using repeated calls for each molecule.
   * @param molecule The molecule the data will be read into.
   * @return True on success, false on failure.
   */
  bool readMolecule(Core::Molecule &molecule);

  /**
   * @brief Read the given @p in stream and load it into @p molecule.
   * @param in The input file stream.
   * @param molecule The molecule the data will be read into.
   * @return True on success, false on failure.
   */
  virtual bool read(std::istream &in, Core::Molecule &molecule) = 0;

  /**
   * @brief Write to the given @p out stream the contents of @p molecule.
   * @param fileName The output stream to write the data to.
   * @param molecule The contents of this molecule will be written to output.
   * @return True on success, false on failure.
   */
  virtual bool write(std::ostream &out, const Core::Molecule &molecule) = 0;

  /**
   * @brief Read the given @p fileName and load it into @p molecule.
   * @param fileName The full path to the file to be read in.
   * @param molecule The molecule the data will be read into.
   * @return True on success, false on failure.
   */
  bool readFile(const std::string &fileName, Core::Molecule &molecule);

  /**
   * @brief Write to the given @p fileName the contents of @p molecule.
   * @param fileName The full path to the file to be written.
   * @param molecule The contents of this molecule will be written to the file.
   * @return True on success, false on failure.
   */
  bool writeFile(const std::string &fileName, const Core::Molecule &molecule);

  /**
   * @brief Read the given @p string and load it into @p molecule.
   * @param string The string containing the molecule file contents.
   * @param molecule The molecule the data will be read into.
   * @return True on success, false on failure.
   */
  bool readString(const std::string &string, Core::Molecule &molecule);

  /**
   * @brief Write to the given @p string the contents of @p molecule.
   * @param string The string to write the contents of the molecule into.
   * @param molecule The contents of this molecule will be written to the string.
   * @return True on success, false on failure.
   */
  bool writeString(std::string &string, const Core::Molecule &molecule);

  /**
   * @brief Get the error string, contains errors/warnings encountered.
   * @return String containing any errors or warnings encountered.
   */
  std::string error() const { return m_error; }

  /**
   * @brief Get the file name (if known).
   * @return The full path to the file name as supplied, can be empty.
   */
  std::string fileName() const { return m_fileName; }

  /**
   * Clear the format and reset all state.
   */
  virtual void clear();

  /**
   * Create a new instance of the file format class. Ownership passes to the
   * caller.
   */
  virtual FileFormat * newInstance() const = 0;

  /**
   * @brief A unique identifier, used to retrieve formats programatically.
   * CML, XYZ, PDB etc. A runtime warning will be generated if the identifier
   * is not unique.
   */
  virtual std::string identifier() const = 0;

  /**
   * @brief The name of the format, should be short such as Chemical Markup
   * Language, XYZ format, Protein Databank etc.
   */
  virtual std::string name() const = 0;

  /**
   * A description of the format, along with any relevant help text for users.
   */
  virtual std::string description() const = 0;

  /**
   * The URL of the format specification if available (relevant web page/wiki
   * otherwise).
   */
  virtual std::string specificationUrl() const = 0;

  /**
   * @brief Get the file name extension(s) that the format supports reading.
   * @return A vector containing a list of extensions (in lower case).
   */
  virtual std::vector<std::string> fileExtensions() const = 0;

  /**
   * @brief Get the MIME type(s) that the format supports reading.
   * @return A vector containing a list of MIME type(s) (in lower case).
   */
  virtual std::vector<std::string> mimeTypes() const = 0;

protected:
  /**
   * @brief Append an error to the error string for the format.
   * @param errorString The error to be added.
   * @param newLine Add a new line after the error string?
   */
  void appendError(const std::string &errorString, bool newLine = true);

private:
  std::string m_error;
  std::string m_fileName;

  // Streams for reading/writing data, especially streaming data in/out.
  Operation m_mode;
  std::istream *m_in;
  std::ostream *m_out;
};

} // end Io namespace
} // end Avogadro namespace

#endif // AVOGADRO_IO_FILEFORMAT_H
