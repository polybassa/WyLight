/**
                Copyright (C) 2012 - 2014 Patrick Bruenn.

    This file is part of WyLight.

    Wylight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    WiflyLight is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with WiflyLight.  If not, see <http://www.gnu.org/licenses/>. */

package de.WyLight.WyLight;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class CopyHelper {
	private final String mOutputDirectory;

	CopyHelper(String outputDirectory) {
		mOutputDirectory = outputDirectory + '/';
	}

	public String copyBufferToFile(byte[] buffer, int bufferLength, String name)
			throws IOException {
		String outputFilename = mOutputDirectory + name;
		FileOutputStream out;
		out = new FileOutputStream(new File(outputFilename));
		out.write(buffer, 0, bufferLength);
		out.close();
		return outputFilename;
	}

	public String copyStreamToFile(InputStream input, String name)
			throws IOException {
		byte[] firmwareBuffer = new byte[1024];
		String outputFilename = mOutputDirectory + name;
		FileOutputStream out;
		out = new FileOutputStream(new File(outputFilename));
		for (int bytesRead = input.read(firmwareBuffer); bytesRead > 0; bytesRead = input
				.read(firmwareBuffer)) {
			System.out.print(firmwareBuffer);
			out.write(firmwareBuffer);
		}
		input.close();
		out.close();
		return outputFilename;
	}
}
